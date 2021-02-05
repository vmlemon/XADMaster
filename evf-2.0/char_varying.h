/* This file provides emulated support for the char_varying data type      */
/* as implemented by the Stratus VOS C compilers.                          */
/* Emulation in C is limited, while emulation in C++ is fairly complete.   */
/*                                                                         */
/* The char_varying data type is widely used in VOS to implement character */
/* strings.  char_varying strings consist of a (short) length word followed*/
/* by the string itself.  There is no trailing null byte.  The maximum     */
/* length of a char_varying is specified at the time it is declared, e.g.  */
/*        char_varying(256) pathname;                                      */
/* VOS C also allows char_varying without a length as an incomplete type.  */
/*                                                                         */
/* VOS C supports a range of operations on char_varying data.  One         */
/* char_varying can be assigned to another, even one with a different      */
/* maximum length.  They can be compared with the arithmetic comparison    */
/* operators, and concatenated with the plus operator.  They can be cast   */
/* to and from any arithmetic type.  And finally, the $substr built-in     */
/* function can be used to manipulate the contents of a char_varying.      */
/*                                                                         */
/* Emulation in C is limited to declaring char_varying's, either with      */
/* or without a length.  To manipulate the contents of the char_varying,   */
/* string functions in <string.h> such as strcpy_vstr_nstr must be used.   */
/*                                                                         */
/* Emulation in C++ permits the following operations:                      */
/*   * declaration of a complete or incomplete char_varying                */
/*   * Initialization of a declaration                                     */
/*   * Assignment                                                          */
/*   * Comparison                                                          */
/*   * Concatenation                                                       */
/* Char string constants (i.e. "abcde") may also participate in these      */
/* operations.                                                             */

#ifndef CHAR_VARYING_H
#define CHAR_VARYING_H

#ifndef __VOSC__
#ifdef __cplusplus

extern "C++" {

/* char_varying is the abstract base class for all char_varying objects. */

class char_varying
{
 protected:
  char_varying () {}                         /* default constructor   */
#ifndef __GNUC__         /* don't define for g++: runtime overhead    */
  ~char_varying () {}                        /* destructor            */
#endif
 private:                                    /* never called          */
  char_varying (const char_varying& src) {}  /* copy constructor      */
  char_varying& operator= (const char_varying& src)
    {                                        /* copy assignment       */
      return *this;
    }
 public:
  inline bool operator== (const char *src2) const;
  inline bool operator!= (const char *src2) const;
  inline bool operator<= (const char *src2) const;
  inline bool operator>= (const char *src2) const;
  inline bool operator<  (const char *src2) const;
  inline bool operator>  (const char *src2) const;
  inline bool operator== (const char_varying& src2) const;
  inline bool operator!= (const char_varying& src2) const;
  inline bool operator<= (const char_varying& src2) const;
  inline bool operator>= (const char_varying& src2) const;
  inline bool operator<  (const char_varying& src2) const;
  inline bool operator>  (const char_varying& src2) const;
 protected:                                  /* utilities             */
  inline void copy   (const long maxlen, const char *src);
  inline void copy   (const long maxlen, const char_varying& src);
  inline void copy   (const long maxlen, const char *src, const long srclen);
  inline void append (const long maxlen, const char *src);
  inline void append (const long maxlen, const char_varying& src);
  inline void append (const long maxlen, const char *src, const long srclen);
 private:
  inline int compare (const char *src2) const;
  inline int compare (const char_varying& src2) const;
  inline int compare (const char *src2, const long src2len) const;
 protected:
  static const long minmaxlen = 1;
  static const long maxmaxlen = 32766;
  short len;
};


/* char_varying objects are created from this template class.         */
/* This class should be referenced via the char_varying macro, below. */

template<short maxlen>
  class _char_varying : public char_varying
{
 public:
  _char_varying () {}                        /* default constructor   */

#ifndef __GNUC__         /* don't define for g++: runtime overhead    */
  ~_char_varying () {}                       /* destructor            */
#endif

  _char_varying (const char *src)
   : char_varying()
    {
      char_varying::copy (maxlen, src);
    }

  _char_varying (const char_varying& src)
   : char_varying()
    {
      char_varying::copy (maxlen, src);
    }

  _char_varying (const _char_varying<maxlen>& src)
   : char_varying()                          /* copy constructor      */
    {
      char_varying::copy (maxlen, src);
    }

  _char_varying& operator= (const char *src)
    {
      char_varying::copy (maxlen, src);
      return *this;
    }

  _char_varying& operator= (const char_varying& src)
    {
      char_varying::copy (maxlen, src);
      return *this;
    }

  _char_varying& operator= (const _char_varying<maxlen>& src)
    {                                        /* copy assignment       */
      char_varying::copy (maxlen, src);
      return *this;
    }

  _char_varying& operator+= (const char *src)
    {
      char_varying::append (maxlen, src);
      return *this;
    }

  _char_varying& operator+= (const char_varying& src)
    {
      char_varying::append (maxlen, src);
      return *this;
    }   

 private:
  char str[maxlen];

 friend
  class char_varying;
};

/* Macro used to create char_varying objects.                         */

#ifndef char_varying
#define char_varying(maxlen_) \
  _char_varying<maxlen_>        
#endif /* char_varying */

/* Implementations.                                                   */

#include <string.h>

/* The relational operators.                                               */

inline bool
  char_varying::operator== (const char *src2) const
  {
    return char_varying::compare(src2) == 0;
  }

inline bool
  char_varying::operator!= (const char *src2) const
  {
    return char_varying::compare(src2) != 0;
  }

inline bool
  char_varying::operator<= (const char *src2) const
  {
    return char_varying::compare(src2) <= 0;
  }

inline bool
  char_varying::operator>= (const char *src2) const
  {
    return char_varying::compare(src2) >= 0;
  }

inline bool
  char_varying::operator< (const char *src2) const
  {
    return char_varying::compare(src2) < 0;
  }

inline bool
  char_varying::operator> (const char *src2) const
  {
    return char_varying::compare(src2) > 0;
  }


inline bool
  char_varying::operator== (const char_varying& src2) const
  {
    return char_varying::compare(src2) == 0;
  }

inline bool
  char_varying::operator!= (const char_varying& src2) const
  {
    return char_varying::compare(src2) != 0;
  }

inline bool
  char_varying::operator<= (const char_varying& src2) const
  {
    return char_varying::compare(src2) <= 0;
  }

inline bool
  char_varying::operator>= (const char_varying& src2) const
  {
    return char_varying::compare(src2) >= 0;
  }

inline bool
  char_varying::operator< (const char_varying& src2) const
  {
    return char_varying::compare(src2) < 0;
  }

inline bool
  char_varying::operator> (const char_varying& src2) const
  {
    return char_varying::compare(src2) > 0;
  }


inline bool
  operator== (const char *src1, const char_varying& src2)
  {
    return src2 == src1;
  }


inline bool
  operator!= (const char *src1, const char_varying& src2)
  {
    return src2 != src1;
  }


inline bool
  operator<= (const char *src1, const char_varying& src2)
  {
    return src2 >= src1;
  }


inline bool
  operator>= (const char *src1, const char_varying& src2)
  {
    return src2 <= src1;
  }


inline bool
  operator< (const char *src1, const char_varying& src2)
  {
    return src2 > src1;
  }


inline bool
  operator> (const char *src1, const char_varying& src2)
{
  return src2 < src1;
}


/* Implementation of the plus (concatenation) operator.               */
/* These return large temporary objects.                              */
/* Better to use a series of += append's, if possible.                */

inline _char_varying<32766>
  operator+ (const char *src1, const char_varying& src2)
#ifdef __GNUG__
    return result;
  {
    result  = src1;
    result += src2;
  }
#else
  {
    _char_varying<32766> result = src1;
    result += src2;
    return result;
  }
#endif


inline _char_varying<32766>
  operator+ (const char_varying& src1, const char *src2)
#ifdef __GNUG__
    return result;
  {
    result  = src1;
    result += src2;
  }
#else
  {
    _char_varying<32766> result = src1;
    result += src2;
    return result;
  }
#endif


inline _char_varying<32766>
  operator+ (const char_varying& src1, const char_varying& src2)
#ifdef __GNUG__
    return result;
  {
    result  = src1;
    result += src2;
  }
#else
  {
    _char_varying<32766> result = src1;
    result += src2;
    return result;
  }
#endif


/* Implementations of the copy and append utility functions.               */

inline void
  char_varying::copy (const long maxlen, const char *src)
  {
    char_varying::copy (maxlen, src, strlen (src));
  }


inline void
  char_varying::copy (const long maxlen, const char_varying& that)
  {
    const _char_varying<maxmaxlen>& src =
        static_cast<const _char_varying<maxmaxlen>&>(that);
    if (this != &that)
      char_varying::copy(maxlen, src.str, src.len);
  }


inline void
  char_varying::copy (const long maxlen, const char *src, const long srclen)
  {
    _char_varying<maxmaxlen>& dest =
        static_cast<_char_varying<maxmaxlen>&>(*this);
    const long n = (srclen < maxlen ? srclen : maxlen);
    memcpy (dest.str, src, n);
    dest.len = n;
  }


inline void
  char_varying::append (const long maxlen, const char *src)
  {
    char_varying::append (maxlen, src, strlen (src));
  }


inline void
  char_varying::append (const long maxlen, const char_varying& that)
  {
    const _char_varying<maxmaxlen>& src =
        static_cast<const _char_varying<maxmaxlen>&>(that);
    char_varying::append (maxlen, src.str, src.len);
  }


inline void
  char_varying::append (const long maxlen, const char *src, const long srclen)
  {
    _char_varying<maxmaxlen>& dest =
        static_cast<_char_varying<maxmaxlen>&>(*this);
    const long n = (srclen < maxlen - dest.len ? srclen : maxlen - dest.len);
    memcpy (dest.str + dest.len, src, n);
    dest.len += n;
  }


/* The compare utility functions return a negative, zero or positive       */
/* result, the same as the strcmp library function.  If one string is      */
/* shorter than the other, though, the longer's tail is compared to        */
/* blanks, the same as VOS C does.                                         */


inline int
  char_varying::compare (const char *src2) const
  {
    return char_varying::compare (src2, strlen (src2));
  }

inline int
  char_varying::compare (const char_varying &that) const
  {
    const _char_varying<maxmaxlen>& src2 =
        static_cast<const _char_varying<maxmaxlen>&>(that);
    return char_varying::compare (src2.str, src2.len);
  }


int
  char_varying::compare (const char *src2, const long src2len) const
  {
    const _char_varying<maxmaxlen>& src1 =
        static_cast<const _char_varying<maxmaxlen>&>(*this);
    const char *src1ptr = src1.str;
    const char *src1end = src1.str + src1.len;
    const char *src2ptr = src2;
    const char *src2end = src2 + src2len;
    unsigned char c1;
    unsigned char c2;

    if (src1.len <= src2len)
      {
        while (src1ptr < src1end)     
          {
            c1 = *src1ptr++;
            c2 = *src2ptr++;
            if (c1 != c2)
              return (c1 - c2);
          }
        c1 = ' ';
        while (src2ptr < src2end)
          {
            c2 = *src2ptr++;
            if (c1 != c2)
              return (c1 - c2);
          }
      }
    else /* src1.len > src2len */
      {
        while (src2ptr < src2end)
          {
            c1 = *src1ptr++;
            c2 = *src2ptr++;
            if (c1 != c2)
              return (c1 - c2);
          }
        c2 = ' ';
        while (src1ptr < src1end)
          {
            c1 = *src1ptr++;
            if (c1 != c2)
              return (c1 - c2);
          }
      }
     
    return 0;

  }

} /* extern "C++" */

#else /* ! __cplusplus */

typedef void char_varying;

#ifndef char_varying
#define char_varying(maxlen_) \
  struct { short len ; char str [(maxlen_)]; }
#endif /* char_varying */

#endif /* !__cplusplus */
#endif /* __VOSC__ */

#endif /* CHAR_VARYING_H */
