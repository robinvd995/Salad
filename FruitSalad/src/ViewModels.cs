using FruitSalad.Director;
using FruitSalad.Util;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace FruitSalad
{
    public class ViewModelBase : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        public void OnPropertyChanged(string propname)
        {
            if(PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(propname));
            }
        }
    }

    public class EditorVM : ViewModelBase
    {
        public EditorVM(FileType type, string filePath)
        {
            Type = type;
            FilePath = filePath;
        }

        public FileType Type { get; private set; }
        public string FilePath { get; private set; }
        public string EditorTitle
        {
            get
            {
                return Type.Name.FirstCharToUpper() + " (" + FilePath.Substring(FilePath.LastIndexOf('\\') + 1) + ")";
            }
        }
    }

    public class EditorTextureVM : EditorVM
    {
        public EditorTextureVM(FileType fileType, string filePath)
            : base(fileType, filePath)
        {
        }
    }

    public class DirectorItemVM : ViewModelBase
    {
        public static readonly ICommand directorActionCommand = new DirectorActionCommand();

        public DirectorItemVM(DirectorItemData data)
        {
            Data = data;
            Children = new List<DirectorItemVM>();
            CommandList = new List<DirectorAction>();

            foreach(Tuple<string,string> tuple in data.ItemType.ActionList)
            {
                DirectorActionParameter parameter = new DirectorActionParameter
                {
                    ActionId = tuple.Item1,
                    ItemData = data,
                };

                CommandList.Add(new DirectorAction()
                {
                    Name = tuple.Item2,
                    Parameter = parameter,
                    Command = directorActionCommand
                });
            }
        }

        public DirectorItemData Data { get; private set; }
        public string ItemName { get { return Data.ItemName; } }
        public string AbsolutePath { get { return Data.AbsolutePath; } }
        public string ImageUrl { get { return Path.GetFullPath(Data.ItemType.GetIcon()); } }
        public DirectoryTreeType ItemType { get { return Data.ItemType; } }
        public List<DirectorItemVM> Children { get; set; }
        public List<DirectorAction> CommandList { get; set; }
        public ICommand DefaultCommand
        {
            get
            {
                return null;
            }
        }
    }

    public class DirectorActionParameter
    {
        public string ActionId { get; set; }
        public DirectorItemData ItemData { get; set; }
    }

    public class DirectorAction
    {
        public string Name { get; set; }
        public ICommand Command { get; set; }
        public DirectorActionParameter Parameter { get; set; }
    }

    public class DirectorActionCommand : ICommand
    {
        public event EventHandler CanExecuteChanged;

        public bool CanExecute(object parameter)
        {
            return true;
        }

        public void Execute(object parameter)
        {
            if(parameter is DirectorActionParameter)
            {
                DirectorActionParameter dap = parameter as DirectorActionParameter;
                dap.ItemData.ItemType.HandleAction(dap.ActionId, dap.ItemData);
            }
        }
    }
}
