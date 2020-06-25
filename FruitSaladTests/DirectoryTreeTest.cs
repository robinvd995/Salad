using System;
using System.Collections.Generic;
using System.IO;
using FruitSalad;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace FruitSaladTests
{
    [TestClass]
    public class DirectoryTreeTest
    {
        private readonly string FilePath;

        public DirectoryTreeTest()
        {
            string dir = Path.GetDirectoryName(Environment.CurrentDirectory);
            string root = Directory.GetParent(dir).FullName;
            FilePath = root + "\\res\\";
        }

        [TestMethod]
        public void DirectoryTreeCreateTest()
        {
            Console.WriteLine("Testing");
           
        }
    }
}
