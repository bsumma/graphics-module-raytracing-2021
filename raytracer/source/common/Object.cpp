//////////////////////////////////////////////////////////////////////////////
//
//  --- Object.cpp ---
//  Created by Brian Summa
//
//////////////////////////////////////////////////////////////////////////////


#include "common.h"

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
Object::IntersectionValues Sphere::intersect(vec4 p0_w, vec4 V_w){
  IntersectionValues result;
  //ray start in object space
  vec4 p0_o = INVC*p0_w;
  //ray direction in object space
  vec4 V_o  = INVCStar*V_w;
  double mag = length(V_o);
  V_o  = normalize(V_o);
  
  //Intersect in object space
  double t_o = raySphereIntersection(p0_o, V_o);
  if(t_o == std::numeric_limits< double >::infinity()){
    result.t_w = std::numeric_limits< double >::infinity();
    return result;
  }

  //t in world space
  result.t_w = t_o/mag;
  
  //intersection point in object space
  vec4 P_o = p0_o + t_o*V_o;
  P_o /= P_o.w;
  
  //intersection point in world space
  result.P_w = p0_w+result.t_w*V_w;
  result.P_w /= result.P_w.w;
  
  //normal in object space
  vec4 N_o = normalize(vec4(P_o.x,P_o.y,P_o.z, 0.0));

  //normal in world space
  vec4 N_w  = TRANINVC * N_o;
  N_w.w = 0;
  result.N_w = normalize(N_w);
  
  return result;
}

/* -------------------------------------------------------------------------- */
/* ------ Ray = p0 + t*V  sphere at origin O and radius r    : Find t ------- */
double Sphere::raySphereIntersection(vec4 p0, vec4 V, vec4 O, double r){
  double t   = std::numeric_limits< double >::infinity();
  double a   = 1.0;
  double b   = dot(2*V, p0-O);
  double c   = (length(p0-O)*length(p0-O)) - (r*r);
  
  double temp = b*b - (4*a*c);
  if(temp < 0.0){ return t;}
  
  double t1 = (-b+sqrt(temp))/(2*a);
  double t2 = (-b-sqrt(temp))/(2*a);
  
  if(t1 < EPSILON){ t1 =  std::numeric_limits< double >::infinity(); }
  if(t2 < EPSILON){ t2 =  std::numeric_limits< double >::infinity(); }
  
  return (t1 < t2)? t1 : t2;
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
Object::IntersectionValues Square::intersect(vec4 p0_w, vec4 V_w){
  IntersectionValues result;
  //ray start in object space
  vec4 p0_o = INVC*p0_w;
  //ray direction in object space
  vec4 V_o  = INVCStar*V_w;
  double mag = length(V_o);
  V_o  = normalize(V_o);
  
  //intersection param in object space
  double t_o = raySquareIntersection(p0_o, V_o);
  if(t_o == std::numeric_limits< double >::infinity()){
    result.t_w = std::numeric_limits< double >::infinity();
    return result;
  }
  
  //intersection param in world space
  result.t_w = t_o/mag;
  
  //intersection point in object space
  vec4 P_o = p0_o+t_o*V_o;
  P_o /= P_o.w;
  
  //intersection point in world space
  result.P_w = p0_w+result.t_w*V_w;
  result.P_w /= result.P_w.w;
  
  //normal in object space
  vec4 N_o = vec4(0.0,0.0,1.0,0.0);
  
  //normal in world space
  vec4 N_w  = TRANINVC*N_o;
  N_w.w = 0;
  result.N_w = normalize(N_w);
  
  return result;
}

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
double Square::raySquareIntersection(vec4 p0, vec4 V){
  vec4 N = vec4(0.0,0.0,1.0, 0.0);
  double t = dot(N, vec4(0.0,0.0,0.0,1.0)-p0)/dot(N, V);
  vec4 temp = p0+t*V;
  
  if(t < EPSILON){ return std::numeric_limits< double >::infinity();}
  
  bool inside_x = (temp.x < (1.0+EPSILON) && temp.x > (-1.0-EPSILON));
  bool inside_y = (temp.y < (1.0+EPSILON) && temp.y > (-1.0-EPSILON));
  
  if(inside_x && inside_y){
    return t;
  }else{
    return std::numeric_limits< double >::infinity();
  }
}
