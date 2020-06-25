using SharpGL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad
{
    public class ShaderContainer
    {
        private readonly List<ShaderSource> shaderSources = new List<ShaderSource>();
        private readonly List<ShaderVariable> uniforms = new List<ShaderVariable>();

        public ShaderContainer()
        {

        }

        public void AppendSource(ShaderSource source)
        {
            shaderSources.Add(source);
        }

        public void CompileShader(OpenGL gl)
        {
            
        }

        public string GetShaderSource(ShaderType type)
        {
            var source = shaderSources.Single(s => s.Type == type);
            if(source == null)
            {
                return "";
            }
            else
            {
                return source.Source;
            }
        }

        public List<ShaderType> GetShaderTypes()
        {
            List<ShaderType> types = new List<ShaderType>();
            foreach(ShaderSource source in shaderSources)
            {
                types.Add(source.Type);
            }
            return types;
        }

        public List<ShaderSource> GetSources()
        {
            return shaderSources;
        }

        public void AddUniform(string uniformName, ShaderDataType type)
        {
            ShaderVariable uniform = new ShaderVariable()
            {
                VariableName = uniformName,
                DataType = type
            };

            if (!ContainsUniform(uniform))
            {
                uniforms.Add(uniform);
            }
            else
            {
                // Error
            }
        }

        public int GetUniformCount()
        {
            return uniforms.Count;
        }

        public ShaderVariable GetUniformAt(int index)
        {
            if (index < 0 || index >= GetUniformCount())
                return null;
            else
                return uniforms.ElementAt(index);
        }

        public bool ContainsUniform(ShaderVariable uniform)
        {
            return uniforms.Where(su => su.VariableName.Equals(uniform.VariableName)).Count() > 0;
        }

        public string Version { get; set; }
    }

    public class ShaderSource
    {
        public ShaderSource(ShaderType type, string source, ShaderType previous)
        {
            Type = type;
            Source = source;
            Previous = previous;
        }

        public ShaderType Type { get; private set; }
        public string Source { get; set; }
        public ShaderType Previous { get; private set; }
    }

    public class ShaderVariable
    {
        public string VariableName { get; set; }
        public ShaderDataType DataType { get; set; }
    }

    public class ShaderDataType : Enumeration
    {
        public static ShaderDataType Float = new ShaderDataType(0, "float");

        public static ShaderDataType Vec2 = new ShaderDataType(1, "vec2");
        public static ShaderDataType Vec3 = new ShaderDataType(2, "vec3");
        public static ShaderDataType Vec4 = new ShaderDataType(3, "vec4");

        public static ShaderDataType Mat2 = new ShaderDataType(4, "mat2");
        public static ShaderDataType Mat3 = new ShaderDataType(5, "mat3");
        public static ShaderDataType Mat4 = new ShaderDataType(6, "mat4");

        public ShaderDataType(int id, string name) :
            base(id, name)
        {
        }
    }

    public class ShaderBuilder {

        private static List<Tuple<string, IDirectiveProcessor>> directiveProcessorList = new List<Tuple<string, IDirectiveProcessor>>();

        private static List<Tuple<string, uint>> shaderTypes = new List<Tuple<string, uint>>() {
            new Tuple<string, uint>("vertex",OpenGL.GL_VERTEX_SHADER),
            new Tuple<string, uint>("fragment",OpenGL.GL_FRAGMENT_SHADER)
        };

        public static uint GetShaderTypeFromString(string type)
        {
            Tuple<string, uint> result = shaderTypes.First(t => t.Item1 == type);
            return result.Item2;
        }

        public static ShaderContainer FromFile(string filePath)
        {
            ShaderContainer shader = new ShaderContainer();

            StringBuilder sourceBuilder = new StringBuilder();
            StringBuilder lineBuilder = new StringBuilder();

            string line;
            System.IO.StreamReader reader = new System.IO.StreamReader(filePath);
            while((line = reader.ReadLine()) != null)
            {
                StringIterator it = new StringIterator(line);
                while (it.HasNext())
                {
                    char c = it.Next();
                    bool shouldAppend = false;
                    if (c == '$')
                    {
                        char nextChar = it.Offset(0);
                        Console.WriteLine("NextChar{0}", nextChar);
                        if(nextChar == '{')
                        {
                            shouldAppend = true;
                        }
                        else
                        {
                            string directiveLine = it.ReadUntilEnd();
                            IEnumerable<string> splitString = directiveLine.Split(' ').Where(s => s.Length > 0);
                            string directive = splitString.First();
                            string[] args = splitString.Skip(1).ToArray();

                            Console.WriteLine("Directive:{0}", directive);
                            IDirectiveProcessor processor = GetDirectiveProcessor(directive);
                           
                            if (processor != null)
                            {
                                bool error = processor.ProcessDirectiveLine(shader, directive, args, lineBuilder, sourceBuilder);
                            }
                            else
                            {
                                // Error, unknown preprocessor directive
                            }
                        }
                    }
                    else if(c == '/')
                    {
                        char nextChar = it.Offset(0);
                        if (nextChar == '/')
                        {
                            string discard = it.ReadUntilEnd();
                            Console.WriteLine("Disacarding Comment: {0}", c + discard);
                        }
                    }
                    else
                    {
                        shouldAppend = true;
                        
                    }

                    if(shouldAppend)
                        lineBuilder.Append(c);
                }
                string sLine = lineBuilder.ToString().Trim();
                sourceBuilder.Append(sLine);
                lineBuilder.Clear();
            }

            reader.Close();

            foreach(var source in shader.GetSources())
            {
                ProcessSource(shader, source);
            }

            return shader;
        }

        private static void ProcessSource(ShaderContainer shader, ShaderSource source)
        {
            StringBuilder stringBuilder = new StringBuilder();
            StringIterator it = new StringIterator(source.Source);

            stringBuilder.Append(shader.Version);

            while (it.HasNext())
            {
                char c = it.Next();
                char nextChar = it.Offset(-2);
                if(c == '{' && nextChar == '$')
                {
                    int scopeDepth = 1;
                    string scopeBlock = "";

                    while (it.HasNext())
                    {
                        char c0 = it.Next();
                        scopeBlock += c0;
                        if (c0 == '{') scopeDepth++;
                        if (c0 == '}' && (--scopeDepth) == 0)
                        {
                            break;
                        }
                    }

                    StringIterator scopeIterator = new StringIterator(scopeBlock);
                    while (scopeIterator.HasNext())
                    {
                        string statement = scopeIterator.ReadUntil(';');
                        stringBuilder.Append(statement + ';');
                    }
                }
                else
                {
                    if(c != '$')
                        stringBuilder.Append(c);
                }
            }

            source.Source = stringBuilder.ToString();
        }

        public static void AddDirectiveProcessor(string identifier, IDirectiveProcessor processor)
        {
            directiveProcessorList.Add(new Tuple<string, IDirectiveProcessor>(identifier, processor));
        }

        public static IDirectiveProcessor GetDirectiveProcessor(string identifier)
        {
            var tuple = directiveProcessorList.Single(t => t.Item1 == identifier);
            if(tuple == null)
            {
                return null;
            }
            return tuple.Item2;
        }
    }

    public interface IDirectiveProcessor
    {
        bool ProcessDirectiveLine(ShaderContainer shader, string directive, string[] args, StringBuilder lineSource, StringBuilder shaderSource);
    }

    public class ShaderType : Enumeration
    {
        public static ShaderType VertexShader = new ShaderType(0, "vertex");
        public static ShaderType FragmentShader = new ShaderType(1, "fragment");

        public ShaderType(int id, string type) :
            base(id, type)
        {

        }
    }

    public class ShaderDirectiveProcessorType : IDirectiveProcessor
    {
        private ShaderType shaderType = null;
        private ShaderType previousType = null;

        public bool ProcessDirectiveLine(ShaderContainer shader, string directive, string[] args, StringBuilder lineSource, StringBuilder shaderSource)
        {
            if(args.Length != 1)
            {
                return false;
            }

            if (args[0].Equals("end"))
            {
                if (shaderType != null)
                {
                    shader.AppendSource(new ShaderSource(shaderType, shaderSource.ToString(), previousType));
                    previousType = shaderType;
                    shaderType = null;
                    shaderSource.Clear();
                    return true;
                }
                else
                {
                    //Error shader type is null
                    return false;
                }
            }

            ShaderType type = Enumeration.GetAll<ShaderType>().Single(st => st.Name.Equals(args[0]));
            if(type == null)
            {
                //Error, unknown shader type {args[0]}
                return false;
            }

            shaderType = type;
            return true;
        }
    }

    public class ShaderDirectiveProcessorVersion : IDirectiveProcessor
    {
        public bool ProcessDirectiveLine(ShaderContainer shader, string directive, string[] args, StringBuilder lineSource, StringBuilder shaderSource)
        {
            string versionString = "#version ";
            foreach(string s in args)
            {
                versionString += s + " ";
            }
            shader.Version = versionString;
            return true;
        }
    }
}
