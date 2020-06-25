using FruitSalad.Assets;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace FruitSalad
{
    /// <summary>
    /// Interaction logic for NewAssetWindow.xaml
    /// </summary>
    public partial class NewAssetWindow : Window
    {
        private readonly NewAssetWindowVM viewModel = new NewAssetWindowVM();

        public NewAssetWindow()
        {
            InitializeComponent();
            viewModel.Description = GetDescriptionForState(viewModel.State);
            DataContext = viewModel;
        }

        private void ButtonClick(object sender, RoutedEventArgs e)
        {
            NewAssetWindowVM vm = DataContext as NewAssetWindowVM;

            if (vm.State == NewAssetWindowState.Select)
            {
                AssetType assetType = vm.SelectedItem.GetAssetType();
                if (assetType.HasSource)
                {
                    vm.AddState(NewAssetWindowState.Source);
                }
                if (assetType.HasProperties)
                {
                    vm.AddState(NewAssetWindowState.Properties);
                }
                vm.AddState(NewAssetWindowState.Finish);
            }

            if (vm.State == NewAssetWindowState.Finish)
            {
                AssetType assetType = vm.SelectedItem.GetAssetType();
                Asset asset = new Asset(FruitSaladApp.Instance.WorkspaceObj.AssetsDirectory + vm.Name, assetType);

                if (assetType.HasSource)
                {
                    asset.SourceFilePath = FruitSaladApp.Instance.WorkspaceObj.SourceDirectory + vm.SourceFile;
                }

                if (assetType.HasProperties)
                {
                    asset.PropertiesFilePath = FruitSaladApp.Instance.WorkspaceObj.PropertiesDirectory + vm.PropertiesFile;
                }

                asset.ToFile();
                Close();
            }
            else
            {
                vm.State = vm.NextState();
                vm.Description = GetDescriptionForState(vm.State);
                DataContext = null;
                DataContext = viewModel;
            }
        }

        public static string GetDescriptionForState(NewAssetWindowState state)
        {
            switch (state)
            {
                default: return "Unknown state!";
                case NewAssetWindowState.Select: return "Select an asset type";
                case NewAssetWindowState.Source: return "Linking Source File";
                case NewAssetWindowState.Properties: return "Linking Properties File";
                case NewAssetWindowState.Finish: return "Linked Files";
            }
        }
    }

    public class NewAssetWindowVM : ViewModelBase
    {
        public List<AssetTypeVM> AssetTypes { get; private set; }
        public AssetTypeVM SelectedItem { get; set; }

        private string name;
        private NewAssetWindowState state = NewAssetWindowState.Select;
        private string sourceFile;
        private string propertiesFile;
        private Queue<NewAssetWindowState> remainingStates = new Queue<NewAssetWindowState>();

        public NewAssetWindowVM()
        {
            //remainingStates.Enqueue(NewAssetWindowState.Select);
            AssetTypes = new List<AssetTypeVM>();

            foreach (AssetType type in Enumeration.GetAll<AssetType>())
            {
                AssetTypes.Add(new AssetTypeVM(type));
            }
        }

        public string Name
        {
            get
            {
                return name;
            }

            set
            {
                name = value;
                OnPropertyChanged("Name");
            }
        }

        public NewAssetWindowState State
        {
            get
            {
                return state;
            }

            set
            {
                state = value;
                OnPropertyChanged("");
                OnPropertyChanged("State");
            }
        }

        public string ButtonText
        {
            get
            {
                return State == NewAssetWindowState.Finish ? "Create" : "Next";
            }
        }

        public string SourceFile
        {
            get
            {
                return sourceFile;
            }

            set
            {
                sourceFile = value;
                OnPropertyChanged("SourceFile");
            }
        }

        public string PropertiesFile
        {
            get
            {
                return propertiesFile;
            }

            set
            {
                propertiesFile = value;
                OnPropertyChanged("PropertiesFile");
            }
        }

        public string Description { get; set; }

        public void AddState(NewAssetWindowState state)
        {
            remainingStates.Enqueue(state);
        }

        public NewAssetWindowState NextState()
        {
            return remainingStates.Dequeue();
        }
    }

    public class AssetTypeVM : ViewModelBase
    {
        private readonly AssetType type;

        public AssetTypeVM(AssetType type)
        {
            this.type = type;
        }

        public string Name
        {
            get
            {
                return type.Name;
            }
        }

        public string Icon
        {
            get
            {
                return "/res/16_16/folder_closed.png";
            }
        }

        public AssetType GetAssetType()
        {
            return type;
        }
    }

    public enum NewAssetWindowState
    {
        Select,Source,Properties,Finish
    }

    public class WindowStateTemplateSelector : DataTemplateSelector
    {
        public override DataTemplate SelectTemplate(object item, DependencyObject container)
        {
            FrameworkElement element = container as FrameworkElement;

            if (element != null && item != null && item is NewAssetWindowVM)
            {
                NewAssetWindowVM vm = item as NewAssetWindowVM;
                NewAssetWindowState state = vm.State;
                string statename = state.ToString().FirstCharToUpper();
                try
                {
                    return element.FindResource("StateTemplate" + statename) as DataTemplate;
                }
                catch (Exception)
                {
                    return null;
                }
            }

            return null;
        }
    }
}
