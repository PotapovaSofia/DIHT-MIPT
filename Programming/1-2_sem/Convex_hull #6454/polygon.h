#include <vector>

struct pt {
	double x, y;
};

class Polygon
{ 
public:
	Polygon();
	Polygon(int size);
	Polygon(vector<pt> &data);
	void Print(Polygon a);
	~Polygon();
private:
	vector<pt> _data;
}