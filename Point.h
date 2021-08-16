#pragma once
#ifndef POINT_H
#define POINT_H


class Point
{
public:
	Point()
	{}

	Point(int x, int y) : m_x(x), m_y(y)
	{}

	Point(const Point &other)
	{
		*this = other;
	}

	~Point()
	{}

	Point &operator=(const Point &other)
	{
		if(this != &other)
		{
			this->m_x = other.m_x;
			this->m_y = other.m_y;
		}
		return *this;
	}

public:
	int m_x = 0, m_y = 0;
};


#endif // !POINT_H
