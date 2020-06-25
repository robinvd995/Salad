using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad
{
    public class FileContainer
    {
        public FileContainer()
        {

        }

        public bool HasPropertyFile { get; set; }
        public string PropertyFilePath { get; set; }
        public PropertyContainer Properties { get; set; }
        public string SourceFilePath { get; set; }
        public SourceContainer Source { get; set; }

        public void SaveFile()
        {
            if (HasPropertyFile)
            {
                Properties.SavePropertiesToFile();
            }
            Source.SaveSource();
        }
    }
}
