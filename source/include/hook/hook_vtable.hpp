#pragma once

#include <memory>

namespace yah::hook {
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
    //! \return pointer to original function, nullptr if we could not hook.
    [[nodiscard]] void* replace_function(size_t function_index, const void* new_function);

    //! Restore a hooked function in the vtable
    //! \param function_index index of function in vtable we want to restore
    //! \return true if function was restored, false if function could not be restored.
    bool restore_function(size_t function_index);

    //! Check if a function has been hooked/replace since we created the hook class.
    //! \param function_index index of function in vtable we want to check.
    //! \return true if function has been hooked, false otherwise.
    [[nodiscard]] bool is_hooked(size_t function_index) const;

    //! Get pointer to original function
    //! \param function_index index of function we want original pointer for.
    //! \return pointer to original function, nullptr if something went wrong.
    [[nodiscard]] void* original_function(size_t function_index) const;

private:
    uintptr_t* _vtable_address = nullptr;
    uintptr_t* _vtable_pointer = nullptr;

    size_t _vtable_size  = 0;

    std::unique_ptr<uintptr_t[]> _vtable_replacement  = nullptr;

};
}
