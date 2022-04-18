#include <algorithm>
#include <dicomcodecs/exception.hpp>
#include <dicomcodecs/image.hpp>
#include <vector>

using namespace dicomcodecs;
using namespace std;

struct ByteWriter {
public:
  ByteWriter(size_t size = 65536) {
    buffer_.resize(0);
    buffer_.reserve(size);
  }
  virtual ~ByteWriter() {}

  void Reset() { buffer_.clear(); }
  size_t Tell() const { return buffer_.size(); }

  void WriteUInt32(uint32_t value) { Write(&value, sizeof(uint32_t)); }
  void WriteInt32(int32_t value) { Write(&value, sizeof(int32_t)); }
  void WriteByte(uint8_t value) { Write(&value, sizeof(uint8_t)); }
  void WriteBytes(uint8_t *values, size_t size) {
    for (auto i = 0; i < size; i++) {
      WriteByte(values[i]);
    }
  }

  uint8_t *GetData() { return buffer_.data(); }

  ByteWriter(ByteWriter const &) = delete;
  ByteWriter &operator=(ByteWriter const &) = delete;

private:
  vector<uint8_t> buffer_;

  size_t Write(void const *ptr, size_t size) {
    auto bytes = reinterpret_cast<uint8_t const *>(ptr);
    buffer_.insert(buffer_.end(), bytes, bytes + size);

    return size;
  }
};

class RleEncoder {
public:
  RleEncoder()
      : segmentCount_(0), prevByte_(-1), repeatCount_(0), bufferPos_(0) {
    for (auto i = 0; i < 15; i++) {
      offsets_[i] = 0;
    }
    for (auto i = 0; i < 132; i++) {
      buffer_[i] = 0;
    }
  }
  ~RleEncoder() {}

  int32_t GetNumberOfSegments() const { return segmentCount_; }
  size_t GetLength() const { return writer_.Tell(); }

  uint8_t *GetBuffer() {
    Flush();
    WriteHeader();

    return writer_.GetData();
  }

  void NextSegment() {
    Flush();
    if ((GetLength() & 1) == 1) {
      writer_.WriteByte(0x00);
    }
    offsets_[segmentCount_++] = writer_.Tell();
  }

  void Encode(uint8_t b) {
    if (b == prevByte_) {
      repeatCount_++;

      if (repeatCount_ > 2 && bufferPos_ > 0) {
        while (bufferPos_ > 0) {
          auto const count = std::min(128, bufferPos_);
          writer_.WriteByte(static_cast<uint8_t>(count - 1));
          MoveBuffer(count);
        }
      } else if (repeatCount_ > 128) {
        auto const count = std::min(repeatCount_, 128);
        writer_.WriteByte(static_cast<uint8_t>(257 - count));
        writer_.WriteByte(static_cast<uint8_t>(prevByte_));
        repeatCount_ -= count;
      }
    } else {
      switch (repeatCount_) {
      case 0:
        break;
      case 1: {
        buffer_[bufferPos_++] = static_cast<uint8_t>(prevByte_);
        break;
      }
      case 2: {
        buffer_[bufferPos_++] = static_cast<uint8_t>(prevByte_);
        buffer_[bufferPos_++] = static_cast<uint8_t>(prevByte_);
        break;
      }
      default: {
        while (repeatCount_ > 0) {
          auto const count = std::min(repeatCount_, 128);
          writer_.WriteByte(static_cast<uint8_t>(257 - count));
          writer_.WriteByte(static_cast<uint8_t>(prevByte_));
          repeatCount_ -= count;
        }
        break;
      }
      }

      while (bufferPos_ > 128) {
        auto const count = std::min(128, bufferPos_);
        writer_.WriteByte(static_cast<uint8_t>(count - 1));
        MoveBuffer(count);
      }

      prevByte_ = b;
      repeatCount_ = 1;
    }
  }

  void MakeEvenLength() {
    if (writer_.Tell() % 2 == 1) {
      writer_.WriteByte(0);
    }
  }

  void Flush() {
    if (repeatCount_ < 2) {
      while (repeatCount_ > 0) {
        buffer_[bufferPos_++] = static_cast<uint8_t>(prevByte_);
        repeatCount_--;
      }
    }

    while (bufferPos_ > 0) {
      auto const count = std::min(128, bufferPos_);
      writer_.WriteByte(static_cast<uint8_t>(count - 1));
      MoveBuffer(count);
    }

    if (repeatCount_ >= 2) {
      while (repeatCount_ > 0) {
        auto const count = std::min(repeatCount_, 128);
        writer_.WriteByte(static_cast<uint8_t>(257 - count));
        writer_.WriteByte(static_cast<uint8_t>(prevByte_));
        repeatCount_ -= count;
      }
    }

    prevByte_ = -1;
    repeatCount_ = 0;
    bufferPos_ = 0;
  }

  RleEncoder(RleEncoder const &) = delete;
  RleEncoder &operator=(RleEncoder const &) = delete;

private:
  uint32_t segmentCount_;
  int32_t offsets_[15];
  uint8_t buffer_[132];
  int32_t prevByte_;
  int32_t repeatCount_;
  int32_t bufferPos_;
  ByteWriter writer_;

  void MoveBuffer(int count) {
    for (auto i = 0; i < count; i++) {
      writer_.WriteByte(buffer_[i]);
    }
    for (auto i = count, n = 0; i < bufferPos_; i++, n++) {
      buffer_[n] = buffer_[i];
    }
    bufferPos_ = bufferPos_ - count;
  }

  void WriteHeader() {
    vector<uint8_t> tmp;
    tmp.assign(writer_.GetData(), writer_.GetData() + writer_.Tell());
    writer_.Reset();

    writer_.WriteUInt32(segmentCount_);
    for (auto i = 0; i < segmentCount_; i++) {
      offsets_[i] += sizeof(uint32_t) + 15 * sizeof(int32_t);
    }
    for (auto i = 0; i < 15; i++) {
      writer_.WriteInt32(offsets_[i]);
    }
    writer_.WriteBytes(tmp.data(), tmp.size());
  }
};

void rleencoder(const image &sourceImage, vector<uint8_t> &encodedBytes) {
  auto const bytesAllocated = (sourceImage.bitsPerSample / 8) +
                              ((sourceImage.bitsPerSample % 8 == 0) ? 0 : 1);
  auto const pixelCount = sourceImage.width * sourceImage.height;
  auto const numberOfSegments = bytesAllocated * sourceImage.componentCount;

  RleEncoder encoder;
  auto frameData = sourceImage.rawBytes.data();

  for (int s = 0; s < numberOfSegments; s++) {
    encoder.NextSegment();

    int sample = s / bytesAllocated;
    int sabyte = s % bytesAllocated;

    int pos;
    int offset;

    if (sourceImage.planarConfiguration == 0) {
      pos = sample * bytesAllocated;
      offset = numberOfSegments;
    } else {
      pos = sample * bytesAllocated * pixelCount;
      offset = bytesAllocated;
    }

    pos += bytesAllocated - sabyte - 1;

    for (int p = 0; p < pixelCount; p++) {
      if ((size_t)pos >= sourceImage.rawBytes.size()) {
        throw dicom_codec_exception(
            "rle", "Read position is past end of frame buffer");
      }
      encoder.Encode(frameData[pos]);
      pos += offset;
    }
    encoder.Flush();
  }

  encoder.MakeEvenLength();
  auto encodedData = encoder.GetBuffer();
  encodedBytes.resize(encoder.GetLength());

  memcpy(encodedBytes.data(), encodedData, encoder.GetLength());
}
