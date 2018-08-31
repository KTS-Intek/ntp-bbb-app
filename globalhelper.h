#ifndef GLOBALHELPER_H
#define GLOBALHELPER_H

#include <QtCore>

class GlobalHelper
{
//    template<typename Container>
public:
    static void lSort(QStringList &c){
        std::sort(c.begin(), c.end());
    }

    static void lSort(QList<QString> &c){
        std::sort(c.begin(), c.end());
    }


    static void lSort(QList<int> &c){
        std::sort(c.begin(), c.end());
    }
    static void lSort(QList<quint8> &c){
        std::sort(c.begin(), c.end());
    }
    static void lSort(QList<quint16> &c){
        std::sort(c.begin(), c.end());
    }
};

#endif // GLOBALHELPER_H
