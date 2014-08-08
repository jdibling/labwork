#include <cstdlib>
#include <vector>
#include <iterator>
#include <iostream>
#include <type_traits>
#include <stdexcept>

/***
 * safe_printf is a typesafe version of sprintf.
 *
 *
 * It works using C++11 techniques to check that 
 * the parameters passed to safe_sprintf and the 
 * format flags in the format string match up.
 *
 * sprintf is not typesafe, because no run-time
 * or compile-time checking is done on the parameters.
 *
 * Consider:
 *
 *  sprintf ("%d", "foo");
 *
 * Here, sprintf is expecting the parameter to be an
 * integral (%d), but we actually pass a const char*
 * ("foo").  This results in Undefined Behavior and,
 * hopefully, a program crash.
 *
 * The way safe_sprintf works is to first call check_printf,
 * which (using recursion) confirms that each parameter
 * is of the type specified by the associated format flag.
 *
 * We are using variadic template expansion here to 
 * construct a call to ValidateArgument on each 
 * parameter, and take the return values from 
 * ValidateArgument to construct the ultimate 
 * parameter list passed to check_printf().
 *
 *
 * Inspirational credit goes to Andrei Alexandrescu.
 *
 */




/**
 * Parameter normalizers for printf.
 * These check that the parameters passed to 
 * safe_printf are types that safe_printf can
 * actually handle.
 *
 * Passing an unsupported parameter to safe_printf 
 * (such as a custom class) will result in a compiler error.
 * There is a commented-out example of this in the 
 * mainline code below.
 *
 * These work by utilizing C++11's SFINAE utility, enable_if,
 * to generate overloads -- and specify return values -- 
 * of normalizeArg() for supported types.
 */

template <class T>
typename std::enable_if <std::is_integral <T>::value, long>::type
ValidateArgument (T arg)
{
  return arg;
}

template <class T>
typename std::enable_if <std::is_floating_point <T>::value, double>::type
ValidateArgument (T arg)
{
  return arg;
}

template <class T>
typename std::enable_if <std::is_pointer <T>::value, T>::type
ValidateArgument (T arg)
{
  return arg;
}

const char*
ValidateArgument(const std::string& str)
{
  return str.c_str();
}


/***
 * check_printf uses recursion to check that each 
 * parameter passed to safe_sprintf is of the type
 * specified in it's format flag.
 *
 * The parameter list passed to check_printf is 
 * constructed by successive calls to ValidateArgument,
 * and is accepted as a a variadic template parameter.
 *
 */

// recursion base case
void check_printf (const char* f)
{
  for (; *f; ++f)
  {
    if (*f != '%' || *++f == '%')
      continue;
    throw std::runtime_error ("Bad format");
  }
}

// recursive check_printf
template <class T, typename... Ts>
void check_printf (
  const char* f,
  const T& t,
  const Ts&... ts)
{
  for (; *f; ++f)
  {
    if (*f != '%' || *++f == '%')
    {
      continue;
    }

    switch (*f)
    {
      case 'f':
      case 'g' :
        if (!std::is_floating_point <T>::value)
          throw std::runtime_error ("Parameter is not floating point!");
        break;
      case 'd' :
        if (!std::is_integral <T>::value)
          throw std::runtime_error ("Parameter is not integral");
        break;
      case 's':
        if (!std::is_object <char*>::value ||
            !std::is_object <std::string>::value)
          throw std::runtime_error ("Non-string parameter");
        break;
      default:
        throw std::runtime_error ("Invalid format character");
    }

    return check_printf (++f, ts...);
  }
}

template <typename... Ts>
int safe_printf (
  const char* f, 
  const Ts&... ts)
{
  check_printf (f, ValidateArgument (ts)...);
  return printf (f, ValidateArgument (ts)...);
}

class Gizmo
{
public:
  // just so Gizmo construction has a side-effect
  Gizmo()
  {
    safe_printf ("Constructed a gizmo...\n");
  }
};

int main()
{
  Gizmo g;
  const std::string foo = "foo";
//  safe_printf ("%s%s%d", g, foo, "bar", 3); // this will not compile because Gizmo is not supported

  safe_printf ("%s%s%d\n", foo, "bar", 3);  // OK

  safe_printf ("%d%s%d", foo, "bar", 3);  // EXCEPTION -- 1st parameter isn't an integer
}


