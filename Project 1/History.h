#ifndef HISTORY_H

#define HISTORY_H

#include "globals.h"

class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;

private:
	int m_numRows;
	int m_numCols;
	int m_HistGrid[MAXROWS][MAXCOLS];
};
#endif
