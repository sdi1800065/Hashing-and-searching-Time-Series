#include "curves.hpp"
#include "hash.hpp"
#include "functionalities.hpp"
#include <cmath>
#include <algorithm>
#include <climits>

int max_dimension;
vector<vector<double>> filtered_curves;
vector<vector<double>> snapped_curves;

void init()         //Init the curves
{
  filtered_curves.resize(p.size());;
  snapped_curves.resize(p.size());
}

vector <double> filter(vector <double> curve)     //This function filters a curve
{
  float a,b,c,e = 0.5;
  int count = 0;
  int i = 0;
  bool deleted = false;
  vector <double> filtered_curve = curve;
  vector <int> indexes_to_be_deleted;
  while (i < filtered_curve.size() - 2) {
     deleted = false;
     a = filtered_curve[i];
     b = filtered_curve[i + 1];
     c = filtered_curve[i + 2];
     if ((abs(a - b) <= e) && (abs(b - c)) <= e) filtered_curve.erase(filtered_curve.begin() + (i+1));      //Erase b if its between a and c
     i++;
  }
  return filtered_curve;
}

vector <double> snap(float t, vector <double> curve)          //This function performs snapping in a curve
{
  int grid;
  float min, max;
  vector <double> snapped_curve;
  for (int i=0; i<curve.size(); i++) {
    grid = floor(curve[i]/delta + 1/2);   //Perform grid formula
    snapped_curve.push_back(grid + t);
  }
  for (int i = 1; i < snapped_curve.size()-1; i++) {
      //Find minima and maxima
     if (snapped_curve[i-1] <= snapped_curve[i+1]) {
       min = snapped_curve[i-1];
       max = snapped_curve[i+1];
     }
     else {
       min =snapped_curve[i+1];
       max = snapped_curve[i-1];
     }
     if(snapped_curve[i] >= min && snapped_curve[i] <= max) snapped_curve.erase(snapped_curve.begin() + i);
  }
  return snapped_curve;
}

vector<double> padding(vector <double> curve, int max_dimension)                  //This function performs padding in a curve
{
  for (int i = curve.size(); i<max_dimension; i++) {
        curve.push_back(99999);                                                 //Push back a very big number
  }
  return curve;
}



//From here starts the github code that was modified to fits with our data types

double distance(vector <double> curve1, vector <double> curve2) {
     //if ((curve1.size() < 2) or (curve2.size() < 2)) {
    //     std::cerr << "WARNING: comparison possible only for curves of at least two points" << std::endl;
    //     Distance result;
    //     result.value = std::numeric_limits<distance_t>::signaling_NaN();
    //     return result;
    // }
    // if (curve1.dimensions() != curve2.dimensions()) {
    //     std::cerr << "WARNING: comparison possible only for curves of equal number of dimensions" << std::endl;
    //     Distance result;
    //     result.value = std::numeric_limits<distance_t>::signaling_NaN();
    //     return result;
     //}
    //
     //const auto start = std::clock();
     //if (Config::verbosity > 2) std::cout << "CFD: computing lower bound" << std::endl;
     float lb = _projective_lower_bound(curve1, curve2);
    // if (Config::verbosity > 2) std::cout << "CFD: computing upper bound" << std::endl;
     float ub = _greedy_upper_bound(curve1, curve2);
    // const auto end = std::clock();
    //
      double dist = _distance(curve1, curve2, ub, lb);
      //auto dist = _distance(curve1, curve2, ub, lb);
    // dist.time_bounds = (end - start) / CLOCKS_PER_SEC;
    //
     return dist;
}

double error = 1;








double _distance(vector <double> curve1, vector <double> curve2, float ub, float lb) {
     double result;
    // const auto start = std::clock();
    //
     float split = (ub + lb)/2;
     const float p_error = lb * error / 100 > std::numeric_limits<double>::epsilon() ? lb * error / 100 : std::numeric_limits<double>::epsilon();
     std::size_t number_searches = 0;
    //
     if (ub - lb > p_error) {
    //     if (Config::verbosity > 2) std::cout << "CFD: binary search using FSD" << std::endl;
    //
         const auto infty = std::numeric_limits<long double>::infinity();
         std::vector<Parameters> reachable1(curve1.size() - 1, Parameters(curve2.size(), infty));
         std::vector<Parameters> reachable2(curve1.size(), Parameters(curve2.size() - 1, infty));
    //
         std::vector<Spans> free_Spans1(curve2.size(), Spans(curve1.size(), Span()));
         std::vector<Spans> free_Spans2(curve1.size(), Spans(curve2.size(), Span()));
    //
         if (std::isnan(lb) or std::isnan(ub)) {
             result = std::numeric_limits<long double>::signaling_NaN();
             return result;
         }
    //
         //Binary search over the feasible distances
         while (ub - lb > p_error) {
             ++number_searches;
             split = (ub + lb)/(long double)(2);
             if (split == lb or split == ub) break;
             auto isLessThan = _less_than_or_equal(split, curve1, curve2, reachable1, reachable2, free_Spans1, free_Spans2);
             if (isLessThan) {
                 ub = split;
             }
             else {
                 lb = split;
             }
    //         if (Config::verbosity > 2) std::cout << "CFD: narrowed distance to to [" << lb << ", " << ub << "]" << std::endl;
         }
     }
    //
    // const auto end = std::clock();
     result = lb;
    // result.time_searches = (end - start) / CLOCKS_PER_SEC;
    // result.number_searches = number_searches;
     return result;
}


bool _less_than_or_equal(const long double distance, vector <double> curve1, vector <double> curve2,
         std::vector<Parameters> &reachable1, std::vector<Parameters> &reachable2,
         std::vector<Spans> &free_Spans1, std::vector<Spans> &free_Spans2) {
    //
    // if (Config::verbosity > 2) std::cout << "CFD: constructing FSD" << std::endl;
     const long double dist_sqr = distance * distance;
     const auto infty = std::numeric_limits<long double>::infinity();
     const int n1 = curve1.size();
     const int n2 = curve2.size();
    //
    // if (Config::verbosity > 2) std::cout << "CFD: resetting old FSD" << std::endl;
    //
     #pragma omp parallel for collapse(2) if (n1 * n2 > 1000)
     for (int i = 0; i < n1; ++i) {
         for (int j = 0; j < n2; ++j) {
             if (i < n1 - 1) reachable1[i][j] = infty;
             if (j < n2 - 1) reachable2[i][j] = infty;
             free_Spans1[j][i].reset();
             free_Spans2[i][j].reset();
         }
     }

    // if (Config::verbosity > 2) std::cout << "CFD: FSD borders" << std::endl;
    //
     for (int  i = 0; i < n1 - 1; ++i) {
         reachable1[i][0] = 0;
         if ((curve2[0] - curve1[i+1]) * (curve2[0] - curve1[i+1]) >  dist_sqr) break;
         // if (curve2[0].dist_sqr(curve1[i+1]) > dist_sqr) break;
     }
    //
     for (int j = 0; j < n2 - 1; ++j) {
         reachable2[0][j] = 0;
         if ((curve1[0] - curve2[j+1]) * (curve1[0] - curve2[j+1]) >  dist_sqr) break;
         //if (curve1[0].dist_sqr(curve2[j+1]) > dist_sqr) break;
     }
    //
    // if (Config::verbosity > 2) std::cout << "CFD: computing free space" << std::endl;
    //
     #pragma omp parallel for collapse(2) if (n1 * n2 > 1000)
     for (int i = 0; i < n1; ++i) {
         for (int j = 0; j < n2; ++j) {
             if ((i < n1 - 1) and (j > 0)) {
                 free_Spans1[j][i] = ball_intersection_Span(dist_sqr, curve1[i], curve1[i+1], curve2[j]);
             }
             if ((j < n2 - 1) and (i > 0)) {
                 free_Spans2[i][j] = ball_intersection_Span(dist_sqr, curve2[j], curve2[j+1], curve1[i]);
             }
         }
     }
    //
    // if (Config::verbosity > 2) std::cout << "CFD: computing reachable space" << std::endl;
    //
     for (int i = 0; i < n1; ++i) {
         for (int j = 0; j < n2; ++j) {
             if ((i < n1 - 1) and (j > 0)) {
                 if (not free_Spans1[j][i].empty()) {
                     if (reachable2[i][j-1] != infty) {
                         reachable1[i][j] = free_Spans1[j][i].begin();
                     }
                     else if (reachable1[i][j-1] <= free_Spans1[j][i].end()) {
                         reachable1[i][j] = std::max(free_Spans1[j][i].begin(), reachable1[i][j-1]);
                     }
                 }
             }
             if ((j < n2 - 1) and (i > 0)) {
                 if (not free_Spans2[i][j].empty()) {
                     if (reachable1[i-1][j] != infty) {
                         reachable2[i][j] = free_Spans2[i][j].begin();
                     }
                     else if (reachable2[i-1][j] <= free_Spans2[i][j].end()) {
                         reachable2[i][j] = std::max(free_Spans2[i][j].begin(), reachable2[i-1][j]);
                     }
                 }
             }
         }
     }
     return reachable1.back().back() < infty;
}

template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, bool> = true>
inline bool near_eq(T x, T y) {
  return std::abs(x - y) <= std::min(std::abs(x), std::abs(y)) * std::numeric_limits<T>::epsilon();
}

Span ball_intersection_Span(const double distance_sqr, const float line_start, const float line_end, float point)  {
         const float u = line_end-line_start, v = point - line_start;
         const long double ulen_sqr = 1, vlen_sqr = 1;
         // const parameter_t ulen_sqr = u.length_sqr(), vlen_sqr = v.length_sqr();
        //
         if (near_eq(ulen_sqr, (long double)(0))) {
             if (vlen_sqr <= distance_sqr) return Span((long double)(0), (long double)(1));
             else return Span();
         }
        //
         const long double p =  -2. * ((u * v) / ulen_sqr), q = vlen_sqr / ulen_sqr - distance_sqr / ulen_sqr;
        //
         const long double phalf_sqr = p * p / 4., discriminant = phalf_sqr - q;
        //
         if (discriminant < 0) return Span();
        //
         const long double discriminant_sqrt = std::sqrt(discriminant);
        //
         const long double minus_p_h = - p / 2., r1 = minus_p_h + discriminant_sqrt, r2 = minus_p_h - discriminant_sqrt;
         const long double lambda1 = std::min(r1, r2), lambda2 = std::max(r1, r2);

         return Span(std::max((long double)(0), lambda1), std::min((long double)(1), lambda2));
}

float _greedy_upper_bound(vector <double> curve1, vector <double> curve2) {
    float result = 0;

    const int len1 = curve1.size(), len2 = curve2.size();
    int i = 0, j = 0;

    while ((i < len1 - 1) and (j < len2 - 1)) {
      //cout << curve1[i] - curve2[j] << endl;
        result = std::max(result, float((curve1[i] - curve2[j]) * (curve1[i] - curve2[j])));
        //result = std::max(result, curve1[i].dist_sqr(curve2[j]));

        float dist1 = (curve1[i+1] - curve2[j])*(curve1[i+1] - curve2[j]) ,
            dist2 = (curve1[i] - curve2[j+1])*(curve1[i] - curve2[j+1]),
            dist3 = (curve1[i+1]-curve2[j+1])*(curve1[i+1]-curve2[j+1]);
        //distance_t dist1 = curve1[i+1].dist_sqr(curve2[j]),
            //dist2 = curve1[i].dist_sqr(curve2[j+1]),
            //dist3 = curve1[i+1].dist_sqr(curve2[j+1]);

        if ((dist1 <= dist2) and (dist1 <= dist3)) ++i;
        else if ((dist2 <= dist1) and (dist2 <= dist3)) ++j;
        else {
            ++i;
            ++j;
        }
    }
    while (i < len1) result = std::max(result, float((curve1[i++] - curve2[j])*(curve1[i++] - curve2[j])));
    // while (i < len1) result = std::max(result, curve1[i++].dist_sqr(curve2[j]));

    --i;

    while (j < len2) result = std::max(result, float((curve1[i] - curve2[j++]) * (curve1[i] - curve2[j++])));
    // while (j < len2) result = std::max(result, curve1[i].dist_sqr(curve2[j++]));

    return std::sqrt(result);
}

float _projective_lower_bound(vector <double> curve1, vector <double> curve2) {
    vector<float> distances1_sqr = vector<float>(curve2.size() - 1), distances2_sqr = vector<float>(curve1.size() + curve2.size() + 2);
    //
     for (int i = 0; i < curve1.size(); ++i) {
         #pragma omp parallel for
         for (int j = 0; j < curve2.size() - 1; ++j) {
             if ((curve2[j] - curve2[j+1]) * (curve2[j] - curve2[j+1])){
             //if (curve2[j].dist_sqr(curve2[j+1]) > 0) {
                  distances1_sqr[j] = line_segment_dist_sqr(curve2[j], curve2[j+1], curve1[i]);
                 //distances1_sqr[j] = curve1[i].line_segment_dist_sqr(curve2[j], curve2[j+1]);
             }
             else {
                   distances1_sqr[j] = ((curve1[i] - curve2[j]) * (curve1[i] - curve2[j]) );
    //             distances1_sqr[j] = curve1[i].dist_sqr(curve2[j]);
             }
         }
         distances2_sqr[i] = *std::min_element(distances1_sqr.begin(), distances1_sqr.end());
     }
    //
     distances1_sqr = std::vector<float>(curve1.size() - 1);
    //
     for (int i = 0; i < curve2.size(); ++i) {
         #pragma omp parallel for
         for (int j = 0; j < curve1.size() - 1; ++j) {
             if (((curve1[j] - curve1[j+1])*(curve1[j] - curve1[j+1])) > 0){
             //if (curve1[j].dist_sqr(curve1[j+1]) > 0) {
                 distances1_sqr[j] = line_segment_dist_sqr(curve1[j], curve1[j+1], curve2[i]);
             } else {
                 distances1_sqr[j] = (curve2[i] - curve1[j]) * (curve2[i] - curve1[j]);
                 //distances1_sqr[j] = curve2[i].dist_sqr(curve1[j]);
             }
         }
         distances2_sqr[curve1.size() + i] = *std::min_element(distances1_sqr.begin(), distances1_sqr.end());
     }

    distances2_sqr[curve1.size() + curve2.size()] = (curve1[0] - curve2[0])*(curve1[0] - curve2[0]);
    // distances2_sqr[curve1.complexity() + curve2.complexity()] = curve1[0].dist_sqr(curve2[0]);
    distances2_sqr[curve1.size() + curve2.size() + 1] = (curve1[curve1.size()-1] - curve2[curve2.size()-1]) * (curve1[curve1.size()-1] - curve2[curve2.size()-1]);
    // distances2_sqr[curve1.complexity() + curve2.complexity() + 1] = curve1[curve1.complexity()-1].dist_sqr(curve2[curve2.complexity()-1]);
    return std::sqrt(*std::max_element(distances2_sqr.begin(), distances2_sqr.end()));
}

float line_segment_dist_sqr(double p1,double p2, double point)
{
  float u = p2-p1;
  float projection_param = (point - p1) * u / (u*u);
  if (projection_param < double(0)) projection_param = double(0);
  else if (projection_param >  double(1)) projection_param = double(1);
  const float projection = p1 + u * projection_param;
  return (point - projection)*(point - projection);
}
