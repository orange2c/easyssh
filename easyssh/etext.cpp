#include "etext.h"

ETEXT::ETEXT( QTextEdit *text_edit, QObject *parent) :
    QObject(parent), Edit( text_edit )
{

}
