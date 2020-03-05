using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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

    public class DirectorItem : ViewModelBase
    {
        private string itemName;
        private FileType itemType = FileType.Unknown;
        private List<DirectorItem> children;
        
        public DirectorItem()
        {
            children = new List<DirectorItem>();
            AbsolutePath = "";
        }

        public string ItemName
        {
            get
            {
                return itemName;
            }
            set
            {
                itemName = value;
                OnPropertyChanged("ItemName");
            }
        }

        public string AbsolutePath { get; set; }

        public string ImageUrl
        {
            get
            {
                return Path.GetFullPath("res/16_16/" + itemType.IconPath);
            }
        }

        public List<DirectorItem> Children
        {
            get
            {
                return children;
            }
            set
            {
                children = value;
                OnPropertyChanged("Children");
            }
        }

        public FileType ItemType
        {
            get
            {
                return itemType;
            }
            set
            {
                itemType = value;
                OnPropertyChanged("ItemType");
                OnPropertyChanged("ImageUrl");
            }
        }
    }
}
