#define CALIBRATION_DEBUG
#include <vector>
#include <string>

struct Point {

public:
	Point(double x, double y) : x(x), y(y) {};
	double x, y;
	void print() {
#ifdef CALIBRATION_DEBUG
		std::cout << "Point print: " << x << " | " << y << std::endl;
#endif
	}
	void print(std::string in) {
#ifdef CALIBRATION_DEBUG
		std::cout << "Point print " << in << ":" << x << " | " << y << std::endl;
#endif
	}
};

typedef Point Vector;

struct Straight {
public:
	Point origin;
	Vector direction;
	Straight(Point origin, Vector direction) : origin(origin), direction(direction) {};
};


struct CalibrationData {

public:

	CalibrationData(int resolutionX, int resolutionY) : calibrationResolutionX(resolutionX), calibrationResolutionY(resolutionY) {
		this->data.clear();
		for (int y1 = 0; y1 < this->calibrationResolutionY; y1++) {
			for (int x1 = 0; x1 < this->calibrationResolutionX; x1++) {
				data.push_back(Point(0, 0));
			}
		}
	};
	~CalibrationData() {};

	//points expected to be in a range from 0.0 to 1.0
	int calibrationResolutionX, calibrationResolutionY;
	std::vector<Point> data;
	int getTotalPoints() { return this->calibrationResolutionX * this->calibrationResolutionY; };
	void setPoint(int where, double x, double y) {
		this->data[where] = Point(x, y);
	}

	Point maxSections() {
		return Point(calibrationResolutionX + 1, calibrationResolutionY + 1);
	}

	Point getSection(Point p) {
		if (p.x <= 1 || p.x >= 0
			&& p.y <= 1 || p.y >= 0) {
			if (p.x > 0.999) {
				p.x = 0.999;
			}if (p.y > 0.999) {
				p.y = 0.999;
			}

			int resultX = 0;
			int resultY = 0;

			double lengthX = 1 / (double)(this->calibrationResolutionX + 1);
			double lengthY = 1 / (double)(this->calibrationResolutionY + 1);

#ifdef CALIBRATION_DEBUG
			std::cout << "LengthX : " << lengthX << std::endl;
			std::cout << "LengthY : " << lengthY << std::endl;
#endif

			int x = 0;
			int y = 0;

			while ((p.x - (lengthX * x)) > lengthX) {
				x += 1;
			}

			while ((p.y - (lengthY * y)) > lengthY) {
				y += 1;
			}

			return Point(x, y);

		}
		return Point(-1, -1);
	}

	Point processPoint(Point point) {
		if (point.x < 1 || point.x > 0
			&& point.y < 1 || point.y > 0) {

			//processes a point according to the touchscreen calibration data
			Point Section = this->getSection(point);

#ifdef CALIBRATION_DEBUG
			std::cout << "Section: " << Section.x << " | " << Section.y << std::endl;
#endif

			double xSize = (1 / ((double)calibrationResolutionX + 1));
			double ySize = (1 / ((double)calibrationResolutionY + 1));

#ifdef CALIBRATION_DEBUG
			std::cout << "X/Y-Size: " << xSize << " | " << ySize << std::endl;
#endif

			int subdivisionsX = this->calibrationResolutionX + 2;
			int subdivisionsY = this->calibrationResolutionY + 2;

#ifdef CALIBRATION_DEBUG
			std::cout << "Subdivisions: " << subdivisionsX << " | " << subdivisionsY << std::endl;
#endif

			std::vector<Point> pop;
			for (int y = 0; y < subdivisionsY; y++) {
				for (int x = 0; x < subdivisionsX; x++) {
					pop.push_back(Point(((double)x) * xSize, ((double)y) * ySize));
				}
			}

#ifdef CALIBRATION_DEBUG
			for (Point d : pop) {
				std::cout << d.x << " | " << d.y << std::endl;
			}
#endif

			for (int y = 0; y < this->calibrationResolutionY; y++) {
				for (int x = 0; x < this->calibrationResolutionX; x++) {
					int thiswhere = (x + (y * this->calibrationResolutionX));

#ifdef CALIBRATION_DEBUG
					std::cout << x << " | " << y << " |- " << thiswhere << " --- " << this->data[thiswhere].x << "|" << this->data[thiswhere].y << std::endl;
#endif

					pop[subdivisionsX + 1 + x + (y * subdivisionsX)] = this->data[thiswhere];
				}
			}

#ifdef CALIBRATION_DEBUG
			for (Point d : pop) {
				std::cout << d.x << " | " << d.y << std::endl;
			}
#endif

			int pointN = (Section.y * subdivisionsX) + Section.x;

#ifdef CALIBRATION_DEBUG
			std::cout << "Calculated Point: " << pointN << std::endl;
			std::cout << "Point is: " << pop[pointN].x << " | " << pop[pointN].y << std::endl;
#endif

			//floatingvalues describe the points position in section 0
			double floatingValueX = point.x;
			double floatingValueY = point.y;
			while (floatingValueX >= (xSize + 0.00001)) {
				floatingValueX = floatingValueX - xSize;
			}
			while (floatingValueY >= (ySize + 0.00001)) {
				floatingValueY = floatingValueY - ySize;
			}
			floatingValueX = floatingValueX * (subdivisionsX - 1);
			floatingValueY = floatingValueY * (subdivisionsY - 1);

#ifdef CALIBRATION_DEBUG
			std::cout << "Point got: " << point.x << " | " << point.y << std::endl;
			std::cout << "Point set: " << floatingValueX << " | " << floatingValueY << std::endl;
#endif

			//vector math incoming UwU
			Point p1 = pop[pointN];
			Point p2 = pop[pointN + 1];
			Point p3 = pop[pointN + subdivisionsY];
			Point p4 = pop[pointN + 1 + subdivisionsY];

			p1.print("P1");
			p2.print("P2");
			p3.print("P3");
			p4.print("P4");

			/*
			1---------2
			|         |
			|         |
			|         |
			|         |
			3---------4
			*/

			/*
			calculate Point PX1 between 1 and 2
			calculate Point PX2 between 3 and 4
			calculate Point PX3 between 1 and 3
			calculate Point PX4 between 2 and 4
			*/

			Point PX1 = Point((p2.x - p1.x) * floatingValueX, (p2.y - p1.y) * floatingValueX);
			Point PX2 = Point((p4.x - p3.x) * floatingValueX, (p4.y - p3.y) * floatingValueX);
			Point PX3 = Point((p3.x - p1.x) * floatingValueY, (p3.y - p1.x) * floatingValueY);
			Point PX4 = Point((p4.x - p2.x) * floatingValueY, (p4.y - p2.y) * floatingValueY);

			PX1.print("PX1");
			PX2.print("PX2");
			PX3.print("PX3");
			PX4.print("PX4");

			/*
			straight VX1 between PX1 and PX2
			straight VX2 between PX3 and PX4
			*/

			Straight VX1 = Straight(PX1, Vector(PX2.x-PX1.x,PX2.y-PX1.y));
			Straight VX2 = Straight(PX3, Vector(PX4.x-PX3.x,PX4.y-PX3.y));

#ifdef CALIBRATION_DEBUG

			std::cout << "Straight One (VX1): " << VX1.origin.x << " | " << VX1.origin.y << " Direction: " << VX1.direction.x << " | " << VX1.direction.y << std::endl;

#endif

			/*
			calculate intersection IX1 between VX1 and VX2
			add IX1 to P1
			*/



			return Point(0, 0);
		}
		return Point(-1, -1);
	}

};
