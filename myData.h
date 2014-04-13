#ifndef DATA_H_H
#define DATA_H_H

class myData
{
	private:
		double dataValue;
		int dataIndex;
	public:
		myData();
		myData(double value_, int index_);
		double getValue() const;
		int getIndex() const;
};

#endif