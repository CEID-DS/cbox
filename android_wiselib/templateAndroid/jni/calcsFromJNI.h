#ifndef CALC_H
#define CALC_H



template <typename T>
class calc
{
  public:
    T multiply(T x, T y);
    T add(T x, T y);
};
template <typename T>
T calc< T >::multiply(T x,T y)
{
  return x*y;
}
template <typename T>
T calc< T >::add(T x, T y)
{
  return x+y;
}

#endif

