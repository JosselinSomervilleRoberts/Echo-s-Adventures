#pragma once

inline float interpolation(float u, float f0, float f1) {
	return f0 + u * (f1 - f0);
}

inline float interpolation2(float u, float f0, float fp0, float f1, float fp1) {
	return (((fp1 - 2 * f1 + fp0 + 2 * f0) * u + (-fp1 + 3 * f1 - 2 * fp0 - 3 * f0)) * u + fp0) * u + f0;
}

inline float interpolation2_derivative(float u, float f0, float fp0, float f1, float fp1) {
	return ((3 * (fp1 - 2 * f1 + fp0 + 2 * f0) * u + 2 * (-fp1 + 3 * f1 - 2 * fp0 - 3 * f0)) * u + fp0);
}







inline size_t find_index_of_interval(float t, vcl::buffer<float> const& intervals)
{
    size_t const N = intervals.size();
    bool error = false;
    if (intervals.size() < 2) {
        std::cout << "Error: Intervals should have at least two values; current size=" << intervals.size() << std::endl;
        error = true;
    }
    if (N > 0 && t < intervals[0]) {
        std::cout << "Error: current time t is smaller than the first time of the interval" << std::endl;
        error = true;
    }
    if (N > 0 && t > intervals[N - 1]) {
        std::cout << "Error: current time t is greater than the last time of the interval" << std::endl;
        error = true;
    }
    if (error == true) {
        std::string const error_str = "Error trying to find interval for t=" + vcl::str(t) + " within values: [" + vcl::str(intervals) + "]";
        error_vcl(error_str);
    }


    size_t k = 0;
    while (intervals[k + 1] < t)
        ++k;
    return k;
}


inline std::vector<vcl::vec3> cardinal_spline_interpolation(float t, float t0, float t1, float t2, float t3, vcl::vec3 const& p0, vcl::vec3 const& p1, vcl::vec3 const& p2, vcl::vec3 const& p3, float K)
{
    // To do: fill the function to compute p(t) as a cardinal spline interpolation
    vcl::vec3  p = { 0,0,0 };

    double s = (t - t1) / (t2 - t1);
    vcl::vec3 di = 2 * K * (p2 - p0) / (t2 - t0);
    vcl::vec3 di2 = 2 * K * (p3 - p1) / (t3 - t1);

    p = (2 * s * s * s - 3 * s * s + 1) * p1 + (s * s * s - 2 * s * s + s) * di + (-2 * s * s * s + 3 * s * s) * p2 + (s * s * s - s * s) * di2;
    vcl::vec3 v = (6 * s * s - 6 * s) * p1 + (3 * s * s - 4 * s + 1) * di + (-6 * s * s + 6 * s) * p2 + (3 * s * s - 2 * s) * di2;
    //v[2] += 0.5;

    std::vector<vcl::vec3> r;
    r.push_back(p);
    r.push_back(normalize(v));
    return r;
}

inline std::vector<vcl::vec3> interpolationPosTime(float t, vcl::buffer<vcl::vec3> const& key_positions, vcl::buffer<float> const& key_times)
{
    // Find idx such that key_times[idx] < t < key_times[idx+1]
    int const idx = find_index_of_interval(t, key_times);

    // Parameters used to compute the linear interpolation
    float const t0 = key_times[std::max(0, idx - 1)]; // = t_{i-1}
    float const t1 = key_times[idx]; // = t_i
    float const t2 = key_times[idx + 1]; // = t_{i+1}
    float const t3 = key_times[std::min(idx + 1, int(key_times.size() - 1))]; // = t_{i+2}

    vcl::vec3 const& p0 = key_positions[std::max(0, idx - 1)]; // = p_{i-1}
    vcl::vec3 const& p1 = key_positions[idx]; // = p_i
    vcl::vec3 const& p2 = key_positions[idx + 1]; // = p_{i+1}
    vcl::vec3 const& p3 = key_positions[std::min(idx + 1, int(key_positions.size() - 1))]; // = p_{i+2}

    double K = 0.4;

    //vec3 const p = linear_interpolation(t, t1,t2, p1,p2);
    return cardinal_spline_interpolation(t, t0, t1, t2, t3, p0, p1, p2, p3, K);
}



inline std::vector<float> cardinal_spline_interpolation(float t, float t0, float t1, float t2, float t3, float const& p0, float const& p1, float const& p2, float const& p3, float K)
{
    // To do: fill the function to compute p(t) as a cardinal spline interpolation
    float p = 0;

    double s = (t - t1) / (t2 - t1);
    float di = 2 * K * (p2 - p0) / (t2 - t0);
    float di2 = 2 * K * (p3 - p1) / (t3 - t1);

    p = (2 * s * s * s - 3 * s * s + 1) * p1 + (s * s * s - 2 * s * s + s) * di + (-2 * s * s * s + 3 * s * s) * p2 + (s * s * s - s * s) * di2;
    float v = (6 * s * s - 6 * s) * p1 + (3 * s * s - 4 * s + 1) * di + (-6 * s * s + 6 * s) * p2 + (3 * s * s - 2 * s) * di2;
    //v[2] += 0.5;

    std::vector<float> r;
    r.push_back(p);
    r.push_back(v);
    return r;
}

inline std::vector<float> interpolationPosTime(float t, vcl::buffer<float> const& key_positions, vcl::buffer<float> const& key_times)
{
    // Find idx such that key_times[idx] < t < key_times[idx+1]
    int const idx = find_index_of_interval(t, key_times);

    // Parameters used to compute the linear interpolation
    float const t0 = key_times[std::max(0, idx - 1)]; // = t_{i-1}
    float const t1 = key_times[idx]; // = t_i
    float const t2 = key_times[idx + 1]; // = t_{i+1}
    float const t3 = key_times[std::min(idx + 1, int(key_times.size() - 1))]; // = t_{i+2}

    float const& p0 = key_positions[std::max(0, idx - 1)]; // = p_{i-1}
    float const& p1 = key_positions[idx]; // = p_i
    float const& p2 = key_positions[idx + 1]; // = p_{i+1}
    float const& p3 = key_positions[std::min(idx + 1, int(key_positions.size() - 1))]; // = p_{i+2}

    double K = 0.4;

    //vec3 const p = linear_interpolation(t, t1,t2, p1,p2);
    return cardinal_spline_interpolation(t, t0, t1, t2, t3, p0, p1, p2, p3, K);
}