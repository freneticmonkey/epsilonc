#include <type_traits>

template <typename T>
struct empty_delete
{
	empty_delete() /* noexcept */
	{
	}

	template <typename U>
	empty_delete(const empty_delete<U>&,
		typename std::enable_if<
		std::is_convertible<U*, T*>::value
		>::type* = nullptr) /* noexcept */
	{
	}

	void operator()(T* const) const /* noexcept */
	{
		// do nothing
	}
};