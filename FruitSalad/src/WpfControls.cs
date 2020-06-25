using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace FruitSalad
{
    class DirectoryTreeView : TreeView
    {
        protected override DependencyObject GetContainerForItemOverride()
        {
            return new DirectoryTreeViewItem();
        }

        protected override bool IsItemItsOwnContainerOverride(object item)
        {
            return item is DirectoryTreeViewItem;
        }
    }

    class DirectoryTreeViewItem : TreeViewItem
    {

        /*protected override void OnMouseDoubleClick(MouseButtonEventArgs e)
        {
            if (IsSelected)
            {
                Console.WriteLine("double click!");
            }
        }*/

        protected override DependencyObject GetContainerForItemOverride()
        {
            return new DirectoryTreeViewItem();
        }

        protected override bool IsItemItsOwnContainerOverride(object item)
        {
            return item is DirectoryTreeViewItem;
        }

        public static DependencyProperty FilePathProperty = DependencyProperty.Register("FilePath", typeof(string), typeof(DirectoryTreeViewItem));

        public string FilePath
        {
            get { return (string)GetValue(FilePathProperty); }
            set { SetValue(FilePathProperty, value); }
        }

        public static DependencyProperty FileTypeProperty = DependencyProperty.Register("FileType", typeof(string), typeof(DirectoryTreeViewItem));

        public string FileType
        {
            get { return (string)GetValue(FileTypeProperty); }
            set { SetValue(FileTypeProperty, value); }
        }
    }
}
