#include <vector>
#include <string>
#include <limits>
using namespace std;
extern float delta;
extern string metric;

extern vector<vector<double>> filtered_curves;
extern vector<vector<double>> snapped_curves;

void init();
vector <double> filter(vector <double> curve);
vector <double> snap(float t, vector <double> curve);
vector<double> padding(vector <double> curve, int max_dimension);


class Span {
     long double beg, en;

public:
    Span() : beg{(long double)(1)}, en{(long double)(0)} {}

    Span(const long double begin, const long double end) : beg{begin}, en{end} {}

    inline bool operator<(const Span &other) const {
        return (beg < other.begin()) or ((beg == other.begin()) and (en < other.end()));
    }

    inline bool empty() const {
        if (en - beg >= std::numeric_limits<long double>::epsilon()) return beg > en;
        else return true;
    }

    inline bool intersects(const Span &other) const {
        if (empty() or other.empty()) return false;

        return ((other.beg >= beg) and (other.beg <= en)) or
            ((other.en >= beg) and (other.en <= en)) or
            ((other.beg <= beg) and (other.en >= en));
    }

    inline long double begin() const {
         return beg;
    }

    inline long double end() const {
        return en;
    }

    inline void reset() {
        beg = (long double)(1);
        en = (long double)(0);
    }
};


typedef vector<long double> Parameters;
typedef vector<Span> Spans;

float _greedy_upper_bound(vector <double> curve1, vector <double> curve2);
float _projective_lower_bound(vector <double> curve1, vector <double> curve2);
double distance(vector <double> curve1, vector <double> curve2);
float line_segment_dist_sqr(double p1,double p2, double point);
double _distance(vector <double> curve1, vector <double> curve2, float ub, float lb);
bool _less_than_or_equal(long double distance, vector <double> curve1, vector <double> curve2,
         std::vector<Parameters> &reachable1, std::vector<Parameters> &reachable2,
         std::vector<Spans> &free_spans1, std::vector<Spans> &free_spans2);
Span ball_intersection_Span(const double distance_sqr, const float line_start, const float line_end, float point);
