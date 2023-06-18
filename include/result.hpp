#ifndef VOID_RRESULT
#define VOID_RRESULT

#include <iostream>
#include <functional>

template <typename T, typename E>
class Result
{
  public:
  Result(const Result& from);

  static Result ok(T value);
  static Result err(E error);
  ~Result();
  T unwrap() const;
  E unwrap_err() const;
  bool is_ok() const;
  bool is_err() const;

  template<typename N>
  Result<N, E> map(std::function<N(T&)> map);
  template<typename N>
  Result<T, N> map_err(std::function<N(E&)> map);
  template<typename N>
  Result<N, E> and_then(std::function<Result<N, E>(T&)> map);
  template<typename N>
  Result<T, N> or_else(std::function<Result<T, N>(E&)> map);

  private:
  Result(T* value, E* error) : value(value), error(error) {};
  T* value;
  E* error;
};

template <typename T, typename E>
Result<T, E>::Result(const Result& from)
{
  this->value = from.value ? new T(*from.value) : nullptr;
  this->error = from.error ? new E(*from.error) : nullptr;
}

template <typename T, typename E>
Result<T, E> Result<T, E>::ok(T value)
{
  E* error = nullptr;
  T* p_value = new T(value);

  return Result<T, E>(p_value, error);
}

template <typename T, typename E>
Result<T, E> Result<T, E>::err(E error)
{
  E* p_error = new E(error);
  T* value = nullptr;

  return Result<T, E>(value, p_error);
}

template <typename T, typename E>
Result<T, E>::~Result()
{
  if (this->value)
    delete this->value;
  else
    delete this->error;
}

template <typename T, typename E>
T Result<T, E>::unwrap() const
{
  if (this->value)
    return *this->value;

  std::cout << "unwrap has not succed!" << '\n';
  exit(EXIT_FAILURE);
}

template <typename T, typename E>
E Result<T, E>::unwrap_err() const
{
  if (this->error)
    return *this->error;

  std::cout << "unwrap has not succed!" << '\n';
  exit(EXIT_FAILURE);
}

template <typename T, typename E>
bool Result<T, E>::is_ok() const
{
  return this->value;
}

template <typename T, typename E>
bool Result<T, E>::is_err() const
{
  return !this->is_ok();
}

template <typename T, typename E>
template<typename N>
Result<N, E> Result<T, E>::map(std::function<N(T&)> map)
{
  if (this->is_ok())
    return Result<N, E>::ok(map(*this->value));
  else
    return Result<N, E>::err(*this->error);
}

template <typename T, typename E>
template<typename N>
Result<T, N> Result<T, E>::map_err(std::function<N(E&)> map)
{
  if (this->is_ok())
    return Result<T, E>::ok(*this->value);
  else
    return Result<T, E>::err(map(*this->error));
}

template <typename T, typename E>
template<typename N>
Result<N, E> Result<T, E>::and_then(std::function<Result<N, E>(T&)> map)
{
  if (this->is_ok())
    return map(*this->value);
  else
    return Result<N, E>::err(*this->error);
}

template <typename T, typename E>
template<typename N>
Result<T, N> Result<T, E>::or_else(std::function<Result<T, N>(E&)> map)
{
  if (this->is_ok())
    return Result::ok(*this->value);
  else
    return map(*this->error)
}

#endif