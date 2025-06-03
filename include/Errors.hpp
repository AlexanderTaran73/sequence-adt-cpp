#pragma once

#include <stdexcept>
#include <string>
#include <utility>

enum class ErrorCode {
    Ok = 0,
    Immutable,
    IndexOutOfRange,
    InvalidArgument,
    EmptyArray,
    EmptyList,
    IncompatibleTypes,
    EmptyValue,
    NegativeSize,
    InvalidIndices,
    NegativeCount,
    NullList,
    ConcatTypeMismatch
};

inline const char* toMessage(ErrorCode code) {
    switch (code) {
        case ErrorCode::Ok: return "Success";
        case ErrorCode::Immutable: return "Immutable object";
        case ErrorCode::IndexOutOfRange: return "Index out of range";
        case ErrorCode::InvalidArgument: return "Invalid argument";
        case ErrorCode::EmptyArray: return "Empty array";
        case ErrorCode::EmptyList: return "Empty list";
        case ErrorCode::IncompatibleTypes: return "Incompatible types";
        case ErrorCode::EmptyValue: return "Empty value";
        case ErrorCode::NegativeSize: return "Negative size not allowed";
        case ErrorCode::InvalidIndices: return "Invalid indices";
        case ErrorCode::NegativeCount: return "Negative count";
        case ErrorCode::NullList: return "Null list";
        case ErrorCode::ConcatTypeMismatch: return "Cannot concat sequences of different types";
        default: return "Unknown error";
    }
}

class BaseError : public std::exception {
private:
    ErrorCode code;
    std::string message;

public:
    BaseError(ErrorCode code_, std::string customMessage = "")
        : code(code_) 
    {
        if (customMessage.empty()) {
            message = toMessage(code);
        } else {
            message = std::string(toMessage(code)) + ": " + std::move(customMessage);
        }
    }

    const char* what() const noexcept override {
        return message.c_str();
    }

    ErrorCode getCode() const noexcept {
        return code;
    }
};

namespace Errors {

    inline BaseError make(ErrorCode code, const std::string& msg = "") {
        return BaseError(code, msg);
    }

    inline BaseError immutable(const std::string& msg = "") {
        return make(ErrorCode::Immutable, msg);
    }

    inline BaseError indexOutOfRange(const std::string& msg = "") {
        return make(ErrorCode::IndexOutOfRange, msg);
    }

    inline BaseError invalidArgument(const std::string& msg = "") {
        return make(ErrorCode::InvalidArgument, msg);
    }

    inline BaseError emptyArray(const std::string& msg = "") {
        return make(ErrorCode::EmptyArray, msg);
    }

    inline BaseError emptyList(const std::string& msg = "") {
        return make(ErrorCode::EmptyList, msg);
    }

    inline BaseError incompatibleTypes(const std::string& msg = "") {
        return make(ErrorCode::IncompatibleTypes, msg);
    }

    inline BaseError emptyValue(const std::string& msg = "") {
        return make(ErrorCode::EmptyValue, msg);
    }

    inline BaseError negativeSize(const std::string& msg = "") {
        return make(ErrorCode::NegativeSize, msg);
    }

    inline BaseError invalidIndices(const std::string& msg = "") {
        return make(ErrorCode::InvalidIndices, msg);
    }

    inline BaseError negativeCount(const std::string& msg = "") {
        return make(ErrorCode::NegativeCount, msg);
    }

    inline BaseError nullList(const std::string& msg = "") {
        return make(ErrorCode::NullList, msg);
    }

    inline BaseError concatTypeMismatch(const std::string& msg = "") {
        return make(ErrorCode::ConcatTypeMismatch, msg);
    }
}
