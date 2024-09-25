#pragma once

#include <cstdint>

#include <expected>
#include <optional>
#include <string_view>

namespace yah {
class hook_vfunc {
public:
    explicit hook_vfunc(uintptr_t* vtable, size_t index);
    ~hook_vfunc();

    hook_vfunc(const hook_vfunc&) = delete;
    hook_vfunc& operator=(const hook_vfunc&) = delete;

    hook_vfunc(hook_vfunc&&) = delete;
    hook_vfunc& operator=(hook_vfunc&&) = delete;

public:
    //! Replace virtual function pointer with our own function pointer
    //! \param new_function address of function to replace in vtable
    //! \return std::expected containing original function pointer or error string
    [[nodiscard]] std::expected<uintptr_t, std::string_view> replace_function(void* new_function) ;

    //! Restore function if hooked.
    //! \return std::expected with fail reason if error
    [[nodiscard]] std::expected<void, std::string_view> restore_function() const;

    //! Check if a function is hooked/has been hooked.
    //! \return true if function has been hooked, false otherwise.
    [[nodiscard]] bool is_hooked() const;

    //! Get pointer to original function
    //! \return std::expected containing original function or error string
    [[nodiscard]] uintptr_t original_function() const;

private:
    uintptr_t* _vtable_pointer  = nullptr;
    size_t _vfunc_index         = 0;

    std::optional<uintptr_t> _vfunc_original_address = std::nullopt;

};
}