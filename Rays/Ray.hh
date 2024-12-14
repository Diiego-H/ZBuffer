#pragma once
#include "library/Common.h"
#include "HitRecord.hh"

class Ray {
  private:
    vec3 origin;
    vec3 direction;
    float tmin, tmax;
    vector<shared_ptr<HitRecord>> hits;

  public:
    Ray() {}

    Ray(const vec3 &orig, const vec3 &dir, float t_min_=0.01f, float t_max_=std::numeric_limits<float>::infinity()):
      origin(orig),
      direction(dir), 
      tmin(t_min_),
      tmax(t_max_)
    {}

    /* retorna el punt del raig en en temps/lambda t */
    vec3 operator() (const float &t) const {
      return origin + direction*t;
    }

    vec3 getOrigin() const       { return origin; }
    vec3 getDirection() const    { return direction; }
    vec3 pointAt(float t) const  { return origin + t*direction; }
    float getTmin() const {return tmin;};
    float getTmax() const {return tmax;};
    void  setTmin(float tmin_) {tmin = tmin_;};
    void  setTmax(float tmax_) {tmax = tmax_;};
    void  addHit(shared_ptr<HitRecord> h) { hits.push_back(h);};
    shared_ptr<HitRecord> getHit(int i) const {return hits[i];};
    vector<shared_ptr<HitRecord>> getHitRecords() const { return hits;};

    ~Ray() {
        hits.clear();
    }

};
