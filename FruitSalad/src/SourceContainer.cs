using FruitSalad.Util;
using ICSharpCode.AvalonEdit.Document;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FruitSalad
{
    public abstract class SourceContainer
    {
        public SourceContainer(string filePath)
        {
            FilePath = filePath;
        }

        public string FilePath { get; private set; }

        public abstract SourceContainerVM GetViewModel();
        public abstract void SaveSource();
    }

    // View Model

    public abstract class SourceContainerVM : ViewModelBase
    {
        public abstract FileType GetFileType();
        public string EditorTitle { get; set; }
    }
}
