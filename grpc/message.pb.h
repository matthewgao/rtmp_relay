// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// source: message.proto
// Protobuf C++ Version: 5.27.2

#ifndef GOOGLE_PROTOBUF_INCLUDED_message_2eproto_2epb_2eh
#define GOOGLE_PROTOBUF_INCLUDED_message_2eproto_2epb_2eh

#include <limits>
#include <string>
#include <type_traits>
#include <utility>

#include "google/protobuf/runtime_version.h"
#if PROTOBUF_VERSION != 5027002
#error "Protobuf C++ gencode is built with an incompatible version of"
#error "Protobuf C++ headers/runtime. See"
#error "https://protobuf.dev/support/cross-version-runtime-guarantee/#cpp"
#endif
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/arena.h"
#include "google/protobuf/arenastring.h"
#include "google/protobuf/generated_message_tctable_decl.h"
#include "google/protobuf/generated_message_util.h"
#include "google/protobuf/metadata_lite.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/message.h"
#include "google/protobuf/repeated_field.h"  // IWYU pragma: export
#include "google/protobuf/extension_set.h"  // IWYU pragma: export
#include "google/protobuf/unknown_field_set.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"

#define PROTOBUF_INTERNAL_EXPORT_message_2eproto

namespace google {
namespace protobuf {
namespace internal {
class AnyMetadata;
}  // namespace internal
}  // namespace protobuf
}  // namespace google

// Internal implementation detail -- do not use these members.
struct TableStruct_message_2eproto {
  static const ::uint32_t offsets[];
};
extern const ::google::protobuf::internal::DescriptorTable
    descriptor_table_message_2eproto;
namespace relay {
namespace api {
class HitWordRequest;
struct HitWordRequestDefaultTypeInternal;
extern HitWordRequestDefaultTypeInternal _HitWordRequest_default_instance_;
class HitWordResponse;
struct HitWordResponseDefaultTypeInternal;
extern HitWordResponseDefaultTypeInternal _HitWordResponse_default_instance_;
}  // namespace api
}  // namespace relay
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google

namespace relay {
namespace api {

// ===================================================================


// -------------------------------------------------------------------

class HitWordResponse final : public ::google::protobuf::Message
/* @@protoc_insertion_point(class_definition:relay.api.HitWordResponse) */ {
 public:
  inline HitWordResponse() : HitWordResponse(nullptr) {}
  ~HitWordResponse() override;
  template <typename = void>
  explicit PROTOBUF_CONSTEXPR HitWordResponse(
      ::google::protobuf::internal::ConstantInitialized);

  inline HitWordResponse(const HitWordResponse& from) : HitWordResponse(nullptr, from) {}
  inline HitWordResponse(HitWordResponse&& from) noexcept
      : HitWordResponse(nullptr, std::move(from)) {}
  inline HitWordResponse& operator=(const HitWordResponse& from) {
    CopyFrom(from);
    return *this;
  }
  inline HitWordResponse& operator=(HitWordResponse&& from) noexcept {
    if (this == &from) return *this;
    if (GetArena() == from.GetArena()
#ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetArena() != nullptr
#endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const
      ABSL_ATTRIBUTE_LIFETIME_BOUND {
    return _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance);
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields()
      ABSL_ATTRIBUTE_LIFETIME_BOUND {
    return _internal_metadata_.mutable_unknown_fields<::google::protobuf::UnknownFieldSet>();
  }

  static const ::google::protobuf::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::google::protobuf::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::google::protobuf::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const HitWordResponse& default_instance() {
    return *internal_default_instance();
  }
  static inline const HitWordResponse* internal_default_instance() {
    return reinterpret_cast<const HitWordResponse*>(
        &_HitWordResponse_default_instance_);
  }
  static constexpr int kIndexInFileMessages = 1;
  friend void swap(HitWordResponse& a, HitWordResponse& b) { a.Swap(&b); }
  inline void Swap(HitWordResponse* other) {
    if (other == this) return;
#ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetArena() != nullptr && GetArena() == other->GetArena()) {
#else   // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetArena() == other->GetArena()) {
#endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::google::protobuf::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(HitWordResponse* other) {
    if (other == this) return;
    ABSL_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  HitWordResponse* New(::google::protobuf::Arena* arena = nullptr) const final {
    return ::google::protobuf::Message::DefaultConstruct<HitWordResponse>(arena);
  }
  using ::google::protobuf::Message::CopyFrom;
  void CopyFrom(const HitWordResponse& from);
  using ::google::protobuf::Message::MergeFrom;
  void MergeFrom(const HitWordResponse& from) { HitWordResponse::MergeImpl(*this, from); }

  private:
  static void MergeImpl(
      ::google::protobuf::MessageLite& to_msg,
      const ::google::protobuf::MessageLite& from_msg);

  public:
  bool IsInitialized() const {
    return true;
  }
  ABSL_ATTRIBUTE_REINITIALIZES void Clear() final;
  ::size_t ByteSizeLong() const final;
  ::uint8_t* _InternalSerialize(
      ::uint8_t* target,
      ::google::protobuf::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::google::protobuf::Arena* arena);
  void SharedDtor();
  void InternalSwap(HitWordResponse* other);
 private:
  friend class ::google::protobuf::internal::AnyMetadata;
  static ::absl::string_view FullMessageName() { return "relay.api.HitWordResponse"; }

 protected:
  explicit HitWordResponse(::google::protobuf::Arena* arena);
  HitWordResponse(::google::protobuf::Arena* arena, const HitWordResponse& from);
  HitWordResponse(::google::protobuf::Arena* arena, HitWordResponse&& from) noexcept
      : HitWordResponse(arena) {
    *this = ::std::move(from);
  }
  const ::google::protobuf::Message::ClassData* GetClassData() const final;

 public:
  ::google::protobuf::Metadata GetMetadata() const;
  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------
  enum : int {
    kRequestIdFieldNumber = 1,
    kMessageFieldNumber = 3,
    kCodeFieldNumber = 2,
  };
  // string requestId = 1;
  void clear_requestid() ;
  const std::string& requestid() const;
  template <typename Arg_ = const std::string&, typename... Args_>
  void set_requestid(Arg_&& arg, Args_... args);
  std::string* mutable_requestid();
  PROTOBUF_NODISCARD std::string* release_requestid();
  void set_allocated_requestid(std::string* value);

  private:
  const std::string& _internal_requestid() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_requestid(
      const std::string& value);
  std::string* _internal_mutable_requestid();

  public:
  // string message = 3;
  void clear_message() ;
  const std::string& message() const;
  template <typename Arg_ = const std::string&, typename... Args_>
  void set_message(Arg_&& arg, Args_... args);
  std::string* mutable_message();
  PROTOBUF_NODISCARD std::string* release_message();
  void set_allocated_message(std::string* value);

  private:
  const std::string& _internal_message() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_message(
      const std::string& value);
  std::string* _internal_mutable_message();

  public:
  // int32 code = 2;
  void clear_code() ;
  ::int32_t code() const;
  void set_code(::int32_t value);

  private:
  ::int32_t _internal_code() const;
  void _internal_set_code(::int32_t value);

  public:
  // @@protoc_insertion_point(class_scope:relay.api.HitWordResponse)
 private:
  class _Internal;
  friend class ::google::protobuf::internal::TcParser;
  static const ::google::protobuf::internal::TcParseTable<
      2, 3, 0,
      50, 2>
      _table_;

  static constexpr const void* _raw_default_instance_ =
      &_HitWordResponse_default_instance_;

  friend class ::google::protobuf::MessageLite;
  friend class ::google::protobuf::Arena;
  template <typename T>
  friend class ::google::protobuf::Arena::InternalHelper;
  using InternalArenaConstructable_ = void;
  using DestructorSkippable_ = void;
  struct Impl_ {
    inline explicit constexpr Impl_(
        ::google::protobuf::internal::ConstantInitialized) noexcept;
    inline explicit Impl_(::google::protobuf::internal::InternalVisibility visibility,
                          ::google::protobuf::Arena* arena);
    inline explicit Impl_(::google::protobuf::internal::InternalVisibility visibility,
                          ::google::protobuf::Arena* arena, const Impl_& from,
                          const HitWordResponse& from_msg);
    ::google::protobuf::internal::ArenaStringPtr requestid_;
    ::google::protobuf::internal::ArenaStringPtr message_;
    ::int32_t code_;
    mutable ::google::protobuf::internal::CachedSize _cached_size_;
    PROTOBUF_TSAN_DECLARE_MEMBER
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_message_2eproto;
};
// -------------------------------------------------------------------

class HitWordRequest final : public ::google::protobuf::Message
/* @@protoc_insertion_point(class_definition:relay.api.HitWordRequest) */ {
 public:
  inline HitWordRequest() : HitWordRequest(nullptr) {}
  ~HitWordRequest() override;
  template <typename = void>
  explicit PROTOBUF_CONSTEXPR HitWordRequest(
      ::google::protobuf::internal::ConstantInitialized);

  inline HitWordRequest(const HitWordRequest& from) : HitWordRequest(nullptr, from) {}
  inline HitWordRequest(HitWordRequest&& from) noexcept
      : HitWordRequest(nullptr, std::move(from)) {}
  inline HitWordRequest& operator=(const HitWordRequest& from) {
    CopyFrom(from);
    return *this;
  }
  inline HitWordRequest& operator=(HitWordRequest&& from) noexcept {
    if (this == &from) return *this;
    if (GetArena() == from.GetArena()
#ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetArena() != nullptr
#endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const
      ABSL_ATTRIBUTE_LIFETIME_BOUND {
    return _internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance);
  }
  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields()
      ABSL_ATTRIBUTE_LIFETIME_BOUND {
    return _internal_metadata_.mutable_unknown_fields<::google::protobuf::UnknownFieldSet>();
  }

  static const ::google::protobuf::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::google::protobuf::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::google::protobuf::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const HitWordRequest& default_instance() {
    return *internal_default_instance();
  }
  static inline const HitWordRequest* internal_default_instance() {
    return reinterpret_cast<const HitWordRequest*>(
        &_HitWordRequest_default_instance_);
  }
  static constexpr int kIndexInFileMessages = 0;
  friend void swap(HitWordRequest& a, HitWordRequest& b) { a.Swap(&b); }
  inline void Swap(HitWordRequest* other) {
    if (other == this) return;
#ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetArena() != nullptr && GetArena() == other->GetArena()) {
#else   // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetArena() == other->GetArena()) {
#endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::google::protobuf::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(HitWordRequest* other) {
    if (other == this) return;
    ABSL_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  HitWordRequest* New(::google::protobuf::Arena* arena = nullptr) const final {
    return ::google::protobuf::Message::DefaultConstruct<HitWordRequest>(arena);
  }
  using ::google::protobuf::Message::CopyFrom;
  void CopyFrom(const HitWordRequest& from);
  using ::google::protobuf::Message::MergeFrom;
  void MergeFrom(const HitWordRequest& from) { HitWordRequest::MergeImpl(*this, from); }

  private:
  static void MergeImpl(
      ::google::protobuf::MessageLite& to_msg,
      const ::google::protobuf::MessageLite& from_msg);

  public:
  bool IsInitialized() const {
    return true;
  }
  ABSL_ATTRIBUTE_REINITIALIZES void Clear() final;
  ::size_t ByteSizeLong() const final;
  ::uint8_t* _InternalSerialize(
      ::uint8_t* target,
      ::google::protobuf::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::google::protobuf::Arena* arena);
  void SharedDtor();
  void InternalSwap(HitWordRequest* other);
 private:
  friend class ::google::protobuf::internal::AnyMetadata;
  static ::absl::string_view FullMessageName() { return "relay.api.HitWordRequest"; }

 protected:
  explicit HitWordRequest(::google::protobuf::Arena* arena);
  HitWordRequest(::google::protobuf::Arena* arena, const HitWordRequest& from);
  HitWordRequest(::google::protobuf::Arena* arena, HitWordRequest&& from) noexcept
      : HitWordRequest(arena) {
    *this = ::std::move(from);
  }
  const ::google::protobuf::Message::ClassData* GetClassData() const final;

 public:
  ::google::protobuf::Metadata GetMetadata() const;
  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------
  enum : int {
    kRequestIdFieldNumber = 1,
    kWordFieldNumber = 2,
    kStartTimeFieldNumber = 3,
    kEndTimeFieldNumber = 4,
  };
  // string requestId = 1;
  void clear_requestid() ;
  const std::string& requestid() const;
  template <typename Arg_ = const std::string&, typename... Args_>
  void set_requestid(Arg_&& arg, Args_... args);
  std::string* mutable_requestid();
  PROTOBUF_NODISCARD std::string* release_requestid();
  void set_allocated_requestid(std::string* value);

  private:
  const std::string& _internal_requestid() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_requestid(
      const std::string& value);
  std::string* _internal_mutable_requestid();

  public:
  // string word = 2;
  void clear_word() ;
  const std::string& word() const;
  template <typename Arg_ = const std::string&, typename... Args_>
  void set_word(Arg_&& arg, Args_... args);
  std::string* mutable_word();
  PROTOBUF_NODISCARD std::string* release_word();
  void set_allocated_word(std::string* value);

  private:
  const std::string& _internal_word() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_word(
      const std::string& value);
  std::string* _internal_mutable_word();

  public:
  // int32 startTime = 3;
  void clear_starttime() ;
  ::int32_t starttime() const;
  void set_starttime(::int32_t value);

  private:
  ::int32_t _internal_starttime() const;
  void _internal_set_starttime(::int32_t value);

  public:
  // int32 endTime = 4;
  void clear_endtime() ;
  ::int32_t endtime() const;
  void set_endtime(::int32_t value);

  private:
  ::int32_t _internal_endtime() const;
  void _internal_set_endtime(::int32_t value);

  public:
  // @@protoc_insertion_point(class_scope:relay.api.HitWordRequest)
 private:
  class _Internal;
  friend class ::google::protobuf::internal::TcParser;
  static const ::google::protobuf::internal::TcParseTable<
      2, 4, 0,
      46, 2>
      _table_;

  static constexpr const void* _raw_default_instance_ =
      &_HitWordRequest_default_instance_;

  friend class ::google::protobuf::MessageLite;
  friend class ::google::protobuf::Arena;
  template <typename T>
  friend class ::google::protobuf::Arena::InternalHelper;
  using InternalArenaConstructable_ = void;
  using DestructorSkippable_ = void;
  struct Impl_ {
    inline explicit constexpr Impl_(
        ::google::protobuf::internal::ConstantInitialized) noexcept;
    inline explicit Impl_(::google::protobuf::internal::InternalVisibility visibility,
                          ::google::protobuf::Arena* arena);
    inline explicit Impl_(::google::protobuf::internal::InternalVisibility visibility,
                          ::google::protobuf::Arena* arena, const Impl_& from,
                          const HitWordRequest& from_msg);
    ::google::protobuf::internal::ArenaStringPtr requestid_;
    ::google::protobuf::internal::ArenaStringPtr word_;
    ::int32_t starttime_;
    ::int32_t endtime_;
    mutable ::google::protobuf::internal::CachedSize _cached_size_;
    PROTOBUF_TSAN_DECLARE_MEMBER
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_message_2eproto;
};

// ===================================================================




// ===================================================================


#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// -------------------------------------------------------------------

// HitWordRequest

// string requestId = 1;
inline void HitWordRequest::clear_requestid() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.requestid_.ClearToEmpty();
}
inline const std::string& HitWordRequest::requestid() const
    ABSL_ATTRIBUTE_LIFETIME_BOUND {
  // @@protoc_insertion_point(field_get:relay.api.HitWordRequest.requestId)
  return _internal_requestid();
}
template <typename Arg_, typename... Args_>
inline PROTOBUF_ALWAYS_INLINE void HitWordRequest::set_requestid(Arg_&& arg,
                                                     Args_... args) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.requestid_.Set(static_cast<Arg_&&>(arg), args..., GetArena());
  // @@protoc_insertion_point(field_set:relay.api.HitWordRequest.requestId)
}
inline std::string* HitWordRequest::mutable_requestid() ABSL_ATTRIBUTE_LIFETIME_BOUND {
  std::string* _s = _internal_mutable_requestid();
  // @@protoc_insertion_point(field_mutable:relay.api.HitWordRequest.requestId)
  return _s;
}
inline const std::string& HitWordRequest::_internal_requestid() const {
  ::google::protobuf::internal::TSanRead(&_impl_);
  return _impl_.requestid_.Get();
}
inline void HitWordRequest::_internal_set_requestid(const std::string& value) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.requestid_.Set(value, GetArena());
}
inline std::string* HitWordRequest::_internal_mutable_requestid() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  return _impl_.requestid_.Mutable( GetArena());
}
inline std::string* HitWordRequest::release_requestid() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  // @@protoc_insertion_point(field_release:relay.api.HitWordRequest.requestId)
  return _impl_.requestid_.Release();
}
inline void HitWordRequest::set_allocated_requestid(std::string* value) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.requestid_.SetAllocated(value, GetArena());
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
        if (_impl_.requestid_.IsDefault()) {
          _impl_.requestid_.Set("", GetArena());
        }
  #endif  // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:relay.api.HitWordRequest.requestId)
}

// string word = 2;
inline void HitWordRequest::clear_word() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.word_.ClearToEmpty();
}
inline const std::string& HitWordRequest::word() const
    ABSL_ATTRIBUTE_LIFETIME_BOUND {
  // @@protoc_insertion_point(field_get:relay.api.HitWordRequest.word)
  return _internal_word();
}
template <typename Arg_, typename... Args_>
inline PROTOBUF_ALWAYS_INLINE void HitWordRequest::set_word(Arg_&& arg,
                                                     Args_... args) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.word_.Set(static_cast<Arg_&&>(arg), args..., GetArena());
  // @@protoc_insertion_point(field_set:relay.api.HitWordRequest.word)
}
inline std::string* HitWordRequest::mutable_word() ABSL_ATTRIBUTE_LIFETIME_BOUND {
  std::string* _s = _internal_mutable_word();
  // @@protoc_insertion_point(field_mutable:relay.api.HitWordRequest.word)
  return _s;
}
inline const std::string& HitWordRequest::_internal_word() const {
  ::google::protobuf::internal::TSanRead(&_impl_);
  return _impl_.word_.Get();
}
inline void HitWordRequest::_internal_set_word(const std::string& value) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.word_.Set(value, GetArena());
}
inline std::string* HitWordRequest::_internal_mutable_word() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  return _impl_.word_.Mutable( GetArena());
}
inline std::string* HitWordRequest::release_word() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  // @@protoc_insertion_point(field_release:relay.api.HitWordRequest.word)
  return _impl_.word_.Release();
}
inline void HitWordRequest::set_allocated_word(std::string* value) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.word_.SetAllocated(value, GetArena());
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
        if (_impl_.word_.IsDefault()) {
          _impl_.word_.Set("", GetArena());
        }
  #endif  // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:relay.api.HitWordRequest.word)
}

// int32 startTime = 3;
inline void HitWordRequest::clear_starttime() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.starttime_ = 0;
}
inline ::int32_t HitWordRequest::starttime() const {
  // @@protoc_insertion_point(field_get:relay.api.HitWordRequest.startTime)
  return _internal_starttime();
}
inline void HitWordRequest::set_starttime(::int32_t value) {
  _internal_set_starttime(value);
  // @@protoc_insertion_point(field_set:relay.api.HitWordRequest.startTime)
}
inline ::int32_t HitWordRequest::_internal_starttime() const {
  ::google::protobuf::internal::TSanRead(&_impl_);
  return _impl_.starttime_;
}
inline void HitWordRequest::_internal_set_starttime(::int32_t value) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.starttime_ = value;
}

// int32 endTime = 4;
inline void HitWordRequest::clear_endtime() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.endtime_ = 0;
}
inline ::int32_t HitWordRequest::endtime() const {
  // @@protoc_insertion_point(field_get:relay.api.HitWordRequest.endTime)
  return _internal_endtime();
}
inline void HitWordRequest::set_endtime(::int32_t value) {
  _internal_set_endtime(value);
  // @@protoc_insertion_point(field_set:relay.api.HitWordRequest.endTime)
}
inline ::int32_t HitWordRequest::_internal_endtime() const {
  ::google::protobuf::internal::TSanRead(&_impl_);
  return _impl_.endtime_;
}
inline void HitWordRequest::_internal_set_endtime(::int32_t value) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.endtime_ = value;
}

// -------------------------------------------------------------------

// HitWordResponse

// string requestId = 1;
inline void HitWordResponse::clear_requestid() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.requestid_.ClearToEmpty();
}
inline const std::string& HitWordResponse::requestid() const
    ABSL_ATTRIBUTE_LIFETIME_BOUND {
  // @@protoc_insertion_point(field_get:relay.api.HitWordResponse.requestId)
  return _internal_requestid();
}
template <typename Arg_, typename... Args_>
inline PROTOBUF_ALWAYS_INLINE void HitWordResponse::set_requestid(Arg_&& arg,
                                                     Args_... args) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.requestid_.Set(static_cast<Arg_&&>(arg), args..., GetArena());
  // @@protoc_insertion_point(field_set:relay.api.HitWordResponse.requestId)
}
inline std::string* HitWordResponse::mutable_requestid() ABSL_ATTRIBUTE_LIFETIME_BOUND {
  std::string* _s = _internal_mutable_requestid();
  // @@protoc_insertion_point(field_mutable:relay.api.HitWordResponse.requestId)
  return _s;
}
inline const std::string& HitWordResponse::_internal_requestid() const {
  ::google::protobuf::internal::TSanRead(&_impl_);
  return _impl_.requestid_.Get();
}
inline void HitWordResponse::_internal_set_requestid(const std::string& value) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.requestid_.Set(value, GetArena());
}
inline std::string* HitWordResponse::_internal_mutable_requestid() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  return _impl_.requestid_.Mutable( GetArena());
}
inline std::string* HitWordResponse::release_requestid() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  // @@protoc_insertion_point(field_release:relay.api.HitWordResponse.requestId)
  return _impl_.requestid_.Release();
}
inline void HitWordResponse::set_allocated_requestid(std::string* value) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.requestid_.SetAllocated(value, GetArena());
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
        if (_impl_.requestid_.IsDefault()) {
          _impl_.requestid_.Set("", GetArena());
        }
  #endif  // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:relay.api.HitWordResponse.requestId)
}

// int32 code = 2;
inline void HitWordResponse::clear_code() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.code_ = 0;
}
inline ::int32_t HitWordResponse::code() const {
  // @@protoc_insertion_point(field_get:relay.api.HitWordResponse.code)
  return _internal_code();
}
inline void HitWordResponse::set_code(::int32_t value) {
  _internal_set_code(value);
  // @@protoc_insertion_point(field_set:relay.api.HitWordResponse.code)
}
inline ::int32_t HitWordResponse::_internal_code() const {
  ::google::protobuf::internal::TSanRead(&_impl_);
  return _impl_.code_;
}
inline void HitWordResponse::_internal_set_code(::int32_t value) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.code_ = value;
}

// string message = 3;
inline void HitWordResponse::clear_message() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.message_.ClearToEmpty();
}
inline const std::string& HitWordResponse::message() const
    ABSL_ATTRIBUTE_LIFETIME_BOUND {
  // @@protoc_insertion_point(field_get:relay.api.HitWordResponse.message)
  return _internal_message();
}
template <typename Arg_, typename... Args_>
inline PROTOBUF_ALWAYS_INLINE void HitWordResponse::set_message(Arg_&& arg,
                                                     Args_... args) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.message_.Set(static_cast<Arg_&&>(arg), args..., GetArena());
  // @@protoc_insertion_point(field_set:relay.api.HitWordResponse.message)
}
inline std::string* HitWordResponse::mutable_message() ABSL_ATTRIBUTE_LIFETIME_BOUND {
  std::string* _s = _internal_mutable_message();
  // @@protoc_insertion_point(field_mutable:relay.api.HitWordResponse.message)
  return _s;
}
inline const std::string& HitWordResponse::_internal_message() const {
  ::google::protobuf::internal::TSanRead(&_impl_);
  return _impl_.message_.Get();
}
inline void HitWordResponse::_internal_set_message(const std::string& value) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.message_.Set(value, GetArena());
}
inline std::string* HitWordResponse::_internal_mutable_message() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  return _impl_.message_.Mutable( GetArena());
}
inline std::string* HitWordResponse::release_message() {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  // @@protoc_insertion_point(field_release:relay.api.HitWordResponse.message)
  return _impl_.message_.Release();
}
inline void HitWordResponse::set_allocated_message(std::string* value) {
  ::google::protobuf::internal::TSanWrite(&_impl_);
  _impl_.message_.SetAllocated(value, GetArena());
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
        if (_impl_.message_.IsDefault()) {
          _impl_.message_.Set("", GetArena());
        }
  #endif  // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:relay.api.HitWordResponse.message)
}

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)
}  // namespace api
}  // namespace relay


// @@protoc_insertion_point(global_scope)

#include "google/protobuf/port_undef.inc"

#endif  // GOOGLE_PROTOBUF_INCLUDED_message_2eproto_2epb_2eh