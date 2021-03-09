#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
using namespace std;
#include "globals.h"
#include "History.h"

History::History(int nRows, int nCols)
{
	m_numRows = nRows;
	m_numCols = nCols;

	for (int r = 0; r < m_numRows; r++)
	{
		for (int c= 0; c < m_numCols; c++)
		{
			m_HistGrid[r][c] = 0;
		}
	}
}

bool History::record(int r, int c)
{
	if (r < 1 || r > m_numRows || c < 1 || c > m_numCols)
		return false;

	m_HistGrid[r-1][c-1]++;
	return true;
}

void History::display() const
{
	clearScreen();

	for (int r = 0; r < m_numRows; r++)
	{
		for (int c = 0; c < m_numCols; c++)
		{
			if (m_HistGrid[r][c] == 0)
				cout << ".";

			else
			{
				char let = 'A' + m_HistGrid[r][c] - 1;
				cout << let;
			}

		}

		cout << endl;
	}

	cout << endl;

}