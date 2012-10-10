#ifndef ISTREAMABLE
#define ISTREAMABLE

#include <QTextStream>

/** 
 * Interface class for objects that can be read from and written to
 * a stream. 
 */
class IStreamable
{
public:
  virtual ~IStreamable(){};
      
  /**
   * Write object properties to an output stream.
   * \param o The output stream to write to.
   * \returns The output stream with the object properties appended
   * to it.
   */
  virtual QTextStream& toStream(QTextStream& o) const = 0; 
  /**
   * Set object properties from an input stream.
   * \param i The input stream to get the properties from.
   * \returns The input stream with the object properties extracted 
   * from it.
   */
  virtual QTextStream& fromStream(QTextStream& i) = 0;
  /** 
   * Write object to stream.
   * 
   * @param o The stream to write to.
   * @param is The object to write.
   * 
   * @return The stream with the object properties appended to it.
   */
  friend QTextStream& operator<< (QTextStream& o, const IStreamable& is);
  /** 
   * Read object properties from stream.
   * 
   * @param i The input stream to get the properties from.
   * @param is The object of which the properties are set.
   * 
   * @return The input stream with the object properties extracted
   * from it
   */
  friend QTextStream& operator>> (QTextStream& i, IStreamable& is);      
      
  static const char space = ' ';
};

inline QTextStream& operator>>(QTextStream& i, IStreamable& is)
{
  is.fromStream(i);
  return i;
}

inline QTextStream& operator<< (QTextStream& o, const IStreamable& is)
{
  is.toStream(o);
  return o;
}



#endif
