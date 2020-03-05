using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad
{
    public class Float3
    {
        public float X { get; set; }
        public float Y { get; set; }
        public float Z { get; set; }

        public static Float3 Parse(string s)
        {
            Float3 float3 = new Float3();
            string[] values = s.Split(' ');
            if(values.Length == 3)
            {
                float3.X = float.Parse(values[0], CultureInfo.InvariantCulture.NumberFormat);
                float3.Y = float.Parse(values[1], CultureInfo.InvariantCulture.NumberFormat);
                float3.Z = float.Parse(values[2], CultureInfo.InvariantCulture.NumberFormat);
                return float3;
            }
            else
            {
                Console.WriteLine("Float3 Parse Error: Invalid number of arguments!");
                return float3;
            }
        }

        public override string ToString()
        {
            return string.Format("Float3({0},{1},{2})", X, Y, Z);
        }
    }
}
