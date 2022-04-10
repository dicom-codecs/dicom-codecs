#include <dicomcodecs/exception.hpp>
#include <dicomcodecs/image.hpp>

#include <cstdint>
#include <cstring>
#include <stdio.h>

using namespace dicomcodecs;
using namespace std;

struct ByteReader {
public:
  ByteReader(uint8_t const *data, size_t size)
      : data_(data), size_(size), pointer_(0) {}
  virtual ~ByteReader() {}

  uint32_t ReadUInt32() {
    uint32_t ret;
    auto dst = reinterpret_cast<uint8_t *>(&ret);
    auto src = const_cast<uint8_t *>(&data_[pointer_]);
    memcpy(dst, src, sizeof(uint32_t));
    pointer_ += sizeof(uint32_t);

    return ret;
  }

  int32_t ReadInt32() {
    int32_t ret;
    auto dst = reinterpret_cast<uint8_t *>(&ret);
    auto src = const_cast<uint8_t *>(&data_[pointer_]);
    memcpy(dst, src, sizeof(int32_t));
    pointer_ += sizeof(int32_t);

    return ret;
  }

  ByteReader(ByteReader const &) = delete;
  ByteReader &operator=(ByteReader const &) = delete;

private:
  uint8_t const *data_;
  size_t size_;
  uint64_t pointer_;
};

class RleDecoder {
public:
  RleDecoder(uint8_t const *data, size_t size)
      : data_(data), size_(size), segmentCount_(0), offsets_(nullptr) {
    ByteReader reader(data, size);
    segmentCount_ = reader.ReadUInt32();
    offsets_ = new int32_t[15];
    for (auto i = 0; i < 15; i++) {
      offsets_[i] = reader.ReadInt32();
    }
  }
  ~RleDecoder() { delete offsets_; }

  int32_t GetNumberOfSegments() const { return segmentCount_; }

  void DecodeSegment(int32_t segment, uint8_t *buffer, size_t size,
                     int32_t start, int32_t sampleOffset) {
    if (segment < 0 || segment >= segmentCount_) {
      throw dicom_codec_exception("rle", "Segment number out of range");
    }
    auto const offset = GetSegmentOffset(segment);
    auto const length = GetSegmentLength(segment);
    Decode(buffer, size, start, sampleOffset, data_, offset, length);
  }

  RleDecoder(RleDecoder const &) = delete;
  RleDecoder &operator=(RleDecoder const &) = delete;

private:
  uint8_t const *data_;
  size_t size_;

  int32_t segmentCount_;
  int32_t *offsets_;

  void Decode(uint8_t *buffer, size_t size, int32_t start, int32_t sampleOffset,
              uint8_t const *rleData, int32_t offset, int32_t count) {
    auto pos = start;
    auto const end = offset + count;
    auto const bufferLength = size;

    for (auto i = offset; i < end && pos < bufferLength;) {
      auto const control = static_cast<int8_t>(rleData[i++]);
      if (control >= 0) {
        auto length = control + 1;
        if ((end - i) < length) {
          throw dicom_codec_exception(
              "rle", "RLE literal run exceeds input buffer length");
        }
        if ((pos + ((length - 1) * sampleOffset)) >= bufferLength) {
          throw dicom_codec_exception(
              "rle", "RLE literal run exceeds output buffer length");
        }
        if (sampleOffset == 1) {
          memcpy(&buffer[pos], &rleData[i], length);
          pos += length;
          i += length;
        } else {
          while (length-- > 0) {
            buffer[pos] = rleData[i++];
            pos += sampleOffset;
          }
        }
      } else if (control >= -127) {
        auto length = -control;
        if ((pos + ((length - 1) * sampleOffset)) >= bufferLength) {
          throw dicom_codec_exception(
              "rle", "RLE repeat run exceeds output buffer length.");
        }
        auto const b = rleData[i++];
        if (sampleOffset == 1) {
          while (length-- >= 0) {
            buffer[pos++] = b;
          }
        } else {
          while (length-- >= 0) {
            buffer[pos] = b;
            pos += sampleOffset;
          }
        }
      }
      if ((i + 1) >= end) {
        break;
      }
    }
  }

  int32_t GetSegmentOffset(int32_t segment) const { return offsets_[segment]; }

  int32_t GetSegmentLength(int32_t segment) const {
    auto const offset = GetSegmentOffset(segment);
    if (segment < (segmentCount_ - 1)) {
      auto const next = GetSegmentOffset(segment + 1);
      return next - offset;
    }

    return size_ - offset;
  }
};

void rledecoder(const vector<uint8_t> &encodedBytes, image &targetImage) {
  RleDecoder decoder(encodedBytes.data(), encodedBytes.size());

  const size_t bytesPerPixel = (targetImage.bitsPerSample + 8 - 1) / 8;
  const size_t destinationSize = targetImage.width * targetImage.height *
                                 targetImage.componentCount * bytesPerPixel;
  targetImage.rawBytes.resize(destinationSize);

  auto const bytesAllocated = (targetImage.bitsPerSample / 8) +
                              ((targetImage.bitsPerSample % 8 == 0) ? 0 : 1);
  auto const pixelCount = targetImage.width * targetImage.height;

  auto const decodedBufferSize =
      pixelCount * bytesAllocated * targetImage.componentCount;
  targetImage.rawBytes.resize(destinationSize);
  auto pDest = targetImage.rawBytes.data();

  for (auto s = 0; s < decoder.GetNumberOfSegments(); s++) {
    auto const sample = s / bytesAllocated;
    auto const sabyte = s % bytesAllocated;

    int32_t pos, offset;
    if (targetImage.planarConfiguration == 0) {
      pos = sample * bytesAllocated;
      offset = targetImage.componentCount * bytesAllocated;
    } else {
      pos = sample * bytesAllocated * pixelCount;
      offset = bytesAllocated;
    }

    pos += bytesAllocated - sabyte - 1;
    decoder.DecodeSegment(s, pDest, decodedBufferSize, pos, offset);
  }
}
