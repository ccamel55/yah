#pragma once

#include <expected>
#include <memory>

namespace yah {
class hook_vtable {
public:
    explicit hook_vtable(uintptr_t* vtable);
    ~hook_vtable();

    hook_vtable(const hook_vtable&) = delete;
    hook_vtable& operator=(const hook_vtable&) = delete;

    hook_vtable(hook_vtable&&) = delete;
    hook_vtable& operator=(hook_vtable&&) = delete;

public:
    //! Replace a function located in the vtable
    //! \param function_index index of function in vtable we want to replace
    //! \param new_function address of function to replace in vtable
    //! \return std::expected containing original function pointer or error string
    [[nodiscard]] std::expected<uintptr_t, std::string_view> replace_function(size_t function_index, void* new_function);

    //! Restore a hooked function in the vtable
    //! \param function_index index of function in vtable we want to restore
    //! \return std::expected containing error string if failed
    [[nodiscard]] std::expected<void, std::string_view> restore_function(size_t function_index);

    //! Check if a function has been hooked/replace since we created the hook class.
    //! \param function_index index of function in vtable we want to check.
    //! \return true if function has been hooked, false otherwise.
    [[nodiscard]] bool is_hooked(size_t function_index) const;

    //! Get pointer to original function
    //! \param function_index index of function we want original pointer for.
    //! \return std::expected containing original function or error string
    [[nodiscard]] std::expected<uintptr_t, std::string_view> original_function(size_t function_index) const;

private:
    uintptr_t* _vtable_address = nullptr;
    uintptr_t* _vtable_pointer = nullptr;

    size_t _vtable_size  = 0;

    std::unique_ptr<uintptr_t[]> _vtable_replacement  = nullptr;

};
}
