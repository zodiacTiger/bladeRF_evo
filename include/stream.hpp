
#include <QtCore>


class Stream
{
 public:
  Stream();
  Stream(const Stream &other);
  ~Stream();

  Stream(const QString &body, const QStringList &headers);

  QString body() const;
  QStringList headers() const;

 private:
  QString m_body;
  QStringList m_headers;
};