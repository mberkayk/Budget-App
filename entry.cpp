#include "entry.h"

Entry::Entry(int a, QString s) {
	amount = a;
	desc = s;
}

int Entry::getAmount(){return amount;}

QString Entry::getDesc(){return desc;}
