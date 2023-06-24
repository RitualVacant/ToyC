//
// Created by lzj on 2023.6.4.
//


/**
 *
 * @param c_
 * @return
 */
bool is_digit(char c_)
{
  if ('0' <= c_ && c_ <= '9')
  {
    return true;
  }
  return false;
}

/**
 *
 * @param c_
 * @return
 */
bool is_alpha(char c_)
{
  if ((c_ >= 'a' && c_ <= 'z') || (c_ >= 'A' && c_ <= 'Z') || c_ == '_')
  {
    return true;
  }
  else
  {
    return false;
  }
}
