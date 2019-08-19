//Include Guard (same as Ifndef)
#pragma once

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int m_asize = 20;

class Numbers
{
//public://For development

	int m_arr[m_asize];
	int i = 0;
	int m_count = 0;
	int ix = 0;
	float m_min = 0;
	float m_max = 0;
	float m_sum = 0;
	float m_avg = 0;
	float temp;

public://For Release

	void Show_Array()
	{
		cout << "(Array\n";
		for (i = 0; i < m_asize; i++)
		{
			cout << "# " << i << " = " << m_arr[i] << endl;
		}
		cout << "  Array)\n";
	}

	//Read file to array
	int Read_From_User_File()
	{
		ifstream file;
		string file_name;
		cout << "Array file name: ";
		cin >> file_name;
		cout << endl;
		///*DEBUG*//cout << "Try to open file " << file_name << endl;
		file.open(file_name.c_str());
		do{ //once
			if (!file)
			{
				cout << "Error opening file!\n";
				break;
			}
			///*DEBUG*//cout << "File opened.\n";
			while (ix < m_asize && file >> m_arr[ix++])
			{
				++m_count;
				///*DEBUG*//cout << ">>#" << ix - 1 << "= " << m_arr[ix - 1] << endl;
			}
			file.close();
		} while (0); //once
		//return # of items read
		return m_count;
	}

	void Calc_Stats()
	{
		//Calculate the minimum from array
		///*DEBUG*//cout << "(Calculate minimum\n";
		for (i = 0,temp = 1000000; i < m_asize; i++)
		{
			if (temp > m_arr[i])
			{
				temp = m_arr[i];
				///*DEBUG*//cout << "Min is: " << temp << endl;
			}
		}
		m_min = temp;
		///*DEBUG*//cout << "  minimum)\n";

		//Calculate the maximum from array
		///*DEBUG*//cout << "(Calculate maximum\n";
		for (i = 0, temp = 0; i < m_asize; i++)
		{
			if (temp < m_arr[i])
			{
				temp = m_arr[i];
				///*DEBUG*//cout << "Max is: " << temp << endl;
			}
		}
		m_max = temp;
		///*DEBUG*//cout << "  maximum)\n";

		//Calculate the sum of the array
		///*DEBUG*//cout << "(Calculate sum\n";
		for (i = 0, temp = 0; i < m_asize; i++)
		{
			temp += m_arr[i];
			///*DEBUG*//cout << "Sum is: " << temp << endl;
		}
		m_sum = temp;
		///*DEBUG*//cout << "  sum)\n";

		//Calculate the average of the array (Use sum and divide by number of elements in the array)
		m_avg = (m_sum / m_asize);
		///*DEBUG*//cout << "Average is: " << m_avg << endl;
	}

	void Show_Stats()
	{
		cout << "Calculated Values:\n"
			<< "Minimum : " << m_min << endl
			<< "Maxiumum: " << m_max << endl
			<< "Sum: " << m_sum << endl
			<< "Average: " << m_avg << endl;
	}
};