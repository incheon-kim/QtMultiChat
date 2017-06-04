#ifndef LIBSMTP_GLOBAL_H
#define LIBSMTP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBSMTP_LIBRARY)
#  define LIBSMTPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBSMTPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBSMTP_GLOBAL_H
