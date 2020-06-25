using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad.Util
{
    public class SplinePoint2f
    {
        public float X { get; set; }
        public float Y { get; set; }
        public float Length { get; set; }
    }

    public class Spline2f
    {
        public List<SplinePoint2f> SplinePoints { get; set; }

        public Spline2f(bool looped)
        {
            SplinePoints = new List<SplinePoint2f>();

            IsLooped = looped;
        }

        public void AddPoint(float px, float py)
        {
            SplinePoints.Add(new SplinePoint2f() { X = px, Y = py, Length = 0.0f });
        }

        public void CalculateSplineLength()
        {
            for(int i = 0; i < SplinePoints.Count; i++)
            {
                CalculateSegmentLength(i);
            }
        }



        public void CalculateSegmentLength(int node)
        {

        }

        public SplinePoint2f GetSplinePoint(float t)
        {
            int p0, p1, p2, p3;
            if (!IsLooped)
            {
                p1 = (int)t + 1;
                p2 = p1 + 1;
                p3 = p2 + 1;
                p0 = p1 - 1;
            }
            else
            {
                p1 = (int)t;
                p2 = (p1 + 1) % SplinePoints.Count;
                p3 = (p2 + 1) % SplinePoints.Count;
                p0 = p1 >= 1 ? p1 - 1 : SplinePoints.Count - 1;
            }

            t = t - (int)t;

            float tt = t * t;
            float ttt = tt * t;

            float q1 = -ttt + 2.0f * tt - t;
            float q2 = 3.0f * ttt - 5.0f * tt + 2.0f;
            float q3 = -3.0f * ttt + 4.0f * tt + t;
            float q4 = ttt - tt;

            float tx = 0.5f * (SplinePoints[p0].X * q1 + SplinePoints[p1].X * q2 + SplinePoints[p2].X * q3 + SplinePoints[p3].X * q4);
            float ty = 0.5f * (SplinePoints[p0].Y * q1 + SplinePoints[p1].Y * q2 + SplinePoints[p2].Y * q3 + SplinePoints[p3].Y * q4);

            return new SplinePoint2f() { X = tx, Y = ty, Length = 1.0f };
        }

        public SplinePoint2f GetSplineGradient(float t)
        {
            int p0, p1, p2, p3;
            if (!IsLooped)
            {
                p1 = (int)t + 1;
                p2 = p1 + 1;
                p3 = p2 + 1;
                p0 = p1 - 1;
            }
            else
            {
                p1 = (int)t + 1;
                p2 = (p1 + 1) % SplinePoints.Count;
                p3 = (p2 + 1) % SplinePoints.Count;
                p0 = p1 >= 1 ? p1 - 1 : SplinePoints.Count - 1;
            }

            float r = t - (int)t;

            float rr = r * r;
            float rrr = rr * r;

            float q0 = -3.0f * rr + 4.0f * r - 1.0f;
            float q1 = 9.0f * rr - 10.0f * r;
            float q2 = -9.0f * rr + 8.0f * r + 1.0f;
            float q3 = 3.0f * rr - 2.0f * t;

            float tx = SplinePoints[p0].X * q0 + SplinePoints[p1].X * q1 + SplinePoints[p2].X * q2 + SplinePoints[p3].X * q3;
            float ty = SplinePoints[p0].Y * q0 + SplinePoints[p1].Y * q1 + SplinePoints[p2].Y * q2 + SplinePoints[p3].Y * q3;

            return new SplinePoint2f() { X = tx, Y = ty, Length = 1.0f };
        }

        public bool IsLooped { get; private set; }
    }
}
