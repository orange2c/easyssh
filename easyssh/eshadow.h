#ifndef ESHADOW_H
#define ESHADOW_H

#include <QObject>
#include <QTextEdit>

class ESHADOW : public QObject
{
    Q_OBJECT
public:
    explicit ESHADOW(QObject *parent = nullptr);

    void inset( QString str );
    void backspace( int count = 1 );

protected:
    QTextEdit *Edit;

signals:

public slots:
};

#endif // ESHADOW_H
