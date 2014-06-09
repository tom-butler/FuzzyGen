#ifndef FLOATPOINT_H
#define FLOATPOINT_H
class FloatPoint
{
  public:
    FloatPoint(float x, float y);
    float pointX;
    float pointY;
};


FloatPoint::FloatPoint(float x, float y) {
  pointX = x;
  pointY = y;
}
#endif