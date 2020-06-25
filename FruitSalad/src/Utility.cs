using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad
{
    public class Parser
    {
        public static float ParseFloat(string value)
        {
            float fvalue = 0.0f;
            try
            {
                fvalue = float.Parse(value, CultureInfo.InvariantCulture.NumberFormat);
            }
            catch (Exception)
            {
                Console.WriteLine("Exception while parsing float!");
            }
            return fvalue;
           
        }

        public static string FloatToString(float value)
        {
            return value.ToString(CultureInfo.InvariantCulture.NumberFormat);
        }
    }

    public class Float2
    {
        public float X { get; set; }
        public float Y { get; set; }

        public static Float2 Parse(string s)
        {
            Float2 float2 = new Float2();
            string[] values = s.Split(' ');
            if (values.Length == 2)
            {
                float2.X = Parser.ParseFloat(values[0]);
                float2.Y = Parser.ParseFloat(values[1]);
                return float2;
            }
            else
            {
                Console.WriteLine("Float2 Parse Error: Invalid number of arguments!");
                return float2;
            }
        }

        public override string ToString()
        {
            return string.Format("Float2({0},{1})", X, Y);
        }

        public string Serialize()
        {
            return X + " " + Y;
        }
    }

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
                float3.X = Parser.ParseFloat(values[0]);
                float3.Y = Parser.ParseFloat(values[1]);
                float3.Z = Parser.ParseFloat(values[2]);
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

        public string Serialize()
        {
            return X + " " + Y + " " + Z;
        }
    }

    public class Float4
    {
        public float X { get; set; }
        public float Y { get; set; }
        public float Z { get; set; }
        public float W { get; set; }

        public static Float4 Parse(string s)
        {
            Float4 float4 = new Float4();
            string[] values = s.Split(' ');
            if (values.Length == 4)
            {
                float4.X = Parser.ParseFloat(values[0]);
                float4.Y = Parser.ParseFloat(values[1]);
                float4.Z = Parser.ParseFloat(values[2]);
                float4.W = Parser.ParseFloat(values[3]);
                return float4;
            }
            else
            {
                Console.WriteLine("Float4 Parse Error: Invalid number of arguments!");
                return float4;
            }
        }

        public override string ToString()
        {
            return string.Format("Float3({0},{1},{2},{3})", X, Y, Z, W);
        }

        public string Serialize()
        {
            return X + " " + Y + " " + Z + " " + W;
        }
    }

    public abstract class Enumeration : IComparable
    {
        public string Name { get; private set; }

        public int Id { get; private set; }

        protected Enumeration(int id, string name)
        {
            Id = id;
            Name = name;
        }

        public override string ToString() => Name;

        public static IEnumerable<T> GetAll<T>() where T : Enumeration
        {
            var fields = typeof(T).GetFields(BindingFlags.Public |
                                             BindingFlags.Static |
                                             BindingFlags.DeclaredOnly);

            return fields.Select(f => f.GetValue(null)).Cast<T>();
        }

        public override bool Equals(object obj)
        {
            var otherValue = obj as Enumeration;

            if (otherValue == null)
                return false;

            var typeMatches = GetType().Equals(obj.GetType());
            var valueMatches = Id.Equals(otherValue.Id);

            return typeMatches && valueMatches;
        }

        public int CompareTo(object other) => Id.CompareTo(((Enumeration)other).Id);

        // Other utility methods ...
    }

    public static class StringExtensions
    {
        public static string FirstCharToUpper(this string input)
        {
            switch (input)
            {
                case null: throw new ArgumentNullException(nameof(input));
                case "": throw new ArgumentException($"{nameof(input)} cannot be empty", nameof(input));
                default: return input.First().ToString().ToUpper() + input.Substring(1);
            }
        }
    }

    public class StringIterator
    {
        private int index = 0;
        private string source;

        public StringIterator(string ssource)
        {
            source = ssource;
        }

        public bool HasNext()
        {
            return index < source.Length;
        }

        public char Next()
        {
            char c = source.ElementAt(index);
            index++;
            return c;
        }

        public char Offset(int offset)
        {
            int i = index + offset;
            if(i >= source.Length || i < 0)
            {
                return ' ';
            }
            return source.ElementAt(i);
        }

        public string ReadUntil(char stopchar)
        {
            string s = "";
            while (HasNext())
            {
                char c = Next();
                if(c == stopchar)
                {
                    break;
                }
                s += c;
            }
            return s;
        }

        public string ReadUntilEnd()
        {
            string s = "";
            while (HasNext())
            {
                char c = Next();
                s += c;
            }
            return s;
        }
    }
}
