using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace FruitSalad
{
    public interface IDialogService
    {
        T OpenDialog<T>(DialogVM<T> viewModel);
    }

    public interface IDialogWindow
    {
        bool? DialogResult { get; set; }
        object DataContext { get; set; }

        bool? ShowDialog();
    }

    public class DialogVM<T>
    {
        public DialogVM(string title, string message)
        {
            Title = title;
            Message = message;
        }

        public string Title { get; private set; }
        public string Message { get; private set; }
        public T DialogResult { get; set; }

        public void CloseDialogWithResult(IDialogWindow dialog, T result)
        {
            DialogResult = result;
            if(dialog != null) 
            { 
                dialog.DialogResult = true;
            }
        }
    }

    public class DialogService : IDialogService
    {
        public T OpenDialog<T>(DialogVM<T> viewModel)
        {
            IDialogWindow window = new DialogWindow
            {
                DataContext = viewModel
            };
            window.ShowDialog();
            return viewModel.DialogResult;
        }
    }

    public enum DialogResult
    {
        Undefined,
        Yes,
        No
    }

    public class RelayCommand<T> : ICommand
    {
        private Action<T> commandAction;
        private Predicate<T> commandPredicate;

        public RelayCommand(Action<T> execute) :
            this(execute, null)
        {
        }

        public RelayCommand(Action<T> execute, Predicate<T> canExecute)
        {
            commandAction = execute;
            commandPredicate = canExecute;
        }
    
        public event EventHandler CanExecuteChanged
        {
            add { CommandManager.RequerySuggested += value; }
            remove { CommandManager.RequerySuggested -= value; }
        }

        public bool CanExecute(object parameter)
        {
            return commandPredicate == null ? true : commandPredicate((T)parameter);
        }

        public void Execute(object parameter)
        {
            commandAction.Invoke((T)parameter);
        }
    }

    public class DialogVMAlert : DialogVM<DialogResult>
    {
        public DialogVMAlert(string title, string message) :
            base(title, message)
        {
            OkCommand = new RelayCommand<IDialogWindow>(Ok);
        }

        public ICommand OkCommand { get; set; }

        public void Ok(IDialogWindow window)
        {
            CloseDialogWithResult(window, DialogResult.Yes);
        }
    }

    public class DialogVMYesNo : DialogVM<DialogResult>
    {
        public DialogVMYesNo(string title, string message) :
            base(title, message)
        {
            YesCommand = new RelayCommand<IDialogWindow>(Yes);
            NoCommand = new RelayCommand<IDialogWindow>(No);
        }

        public ICommand YesCommand { get; set; }
        public ICommand NoCommand { get; set; }

        public void Yes(IDialogWindow window)
        {
            CloseDialogWithResult(window, DialogResult.Yes);
        }

        public void No(IDialogWindow window)
        {
            CloseDialogWithResult(window, DialogResult.No);
        }
    }
}
