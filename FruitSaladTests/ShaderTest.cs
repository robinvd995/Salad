using System;
using System.Text;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using FruitSalad;
using System.IO;

namespace FruitSaladTests
{
    [TestClass]
    public class ShaderTest
    {
        private readonly string ShaderFile;

        public ShaderTest()
        {
            string dir = Path.GetDirectoryName(Environment.CurrentDirectory);
            string root = Directory.GetParent(dir).FullName;
            ShaderFile = root + "\\res\\TestShader.glsl";
        }

        private TestContext testContextInstance;

        public TestContext TestContext
        {
            get
            {
                return testContextInstance;
            }
            set
            {
                testContextInstance = value;
            }
        }

        #region Additional test attributes
        //
        // You can use the following additional attributes as you write your tests:
        //
        // Use ClassInitialize to run code before running the first test in the class
        // [ClassInitialize()]
        // public static void MyClassInitialize(TestContext testContext) { }
        //
        // Use ClassCleanup to run code after all tests in a class have run
        // [ClassCleanup()]
        // public static void MyClassCleanup() { }
        //
        // Use TestInitialize to run code before running each test 
        // [TestInitialize()]
        // public void MyTestInitialize() { }
        //
        // Use TestCleanup to run code after each test has run
        // [TestCleanup()]
        // public void MyTestCleanup() { }
        //
        #endregion

        [TestMethod]
        public void ShaderCompilationTest()
        {
            ShaderBuilder.AddDirectiveProcessor("type", new ShaderDirectiveProcessorType());
            ShaderBuilder.AddDirectiveProcessor("version", new ShaderDirectiveProcessorVersion());
            ShaderContainer shader = ShaderBuilder.FromFile(ShaderFile);

            Console.WriteLine("VertexShader={0}", shader.GetShaderSource(ShaderType.VertexShader));
            Console.WriteLine("FragmentShader={0}", shader.GetShaderSource(ShaderType.FragmentShader));
            
            for(int i = 0; i < shader.GetUniformCount(); i++)
            {
                ShaderVariable uniform = shader.GetUniformAt(i);
                Console.WriteLine("Uniform({0},{1})", uniform.VariableName, uniform.DataType.Name);
            }
        }
    }
}
