#ifndef ERR_H_INCLUDED
#define ERR_H_INCLUDED

#include "utils/ins.h"
#include <sstream>
#include <string>

struct Err
{
    enum class ErrType
    {
        NONE,

        // interpreter
        UNKNOWN_INSTRUCTION,
        INVALID_INS_FORMAT,
    };

    Err(const ErrType& err_type, const std::string& msg, const size_t pc,
        const Ins& ins)
        : err_type_(err_type), msg_(msg), pc_(pc), ins_(ins)
    {}

    Err(const Err& err)
        : err_type_(err.err_type_), msg_(err.msg_), pc_(err.pc_),
          ins_(err.ins_)
    {}

    Err() = default;

    std::string ToString() const
    {
        std::stringstream err_msg;
        err_msg << "[ERROR] at pc " << pc_ << "\n";
        err_msg << msg_ + "\n";
        err_msg << "[INSTRUCTION DUMP]\n";
        err_msg << ins_.ToString() << "\n";

        return err_msg.str();
    }

    ErrType err_type_ = ErrType::NONE;
    std::string msg_{};
    size_t pc_{};
    Ins ins_{};
};

#endif