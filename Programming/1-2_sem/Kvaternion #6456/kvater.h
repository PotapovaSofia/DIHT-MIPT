class kvaternion
{
public:
	kvaternion(double const x = 0);
	kvaternion(double a, double b, double c, double d);
	~kvaternion();

	kvaternion operator +  (const kvaternion &) const;
	kvaternion operator -  (const kvaternion &) const;
	kvaternion operator *  (const kvaternion &) const;
	kvaternion operator /  (const kvaternion &) const;

	void operator = (const kvaternion &);
	void operator += (const kvaternion &);
	void operator *= (const kvaternion &);
	void operator /= (const kvaternion &);

	bool operator == (const kvaternion &other) const;

	double module (const kvaternion &) const;
	kvaternion conjugate (const kvaternion &) const;
	double dot (const kvaternion &kv1, const kvaternion &kv2) const;
	kvaternion cross (const kvaternion &kv1, const kvaternion &kv2) const;
	void print(const kvaternion &kv);

private:
	double _a;
	double _b;
	double _c;
	double _d;
}