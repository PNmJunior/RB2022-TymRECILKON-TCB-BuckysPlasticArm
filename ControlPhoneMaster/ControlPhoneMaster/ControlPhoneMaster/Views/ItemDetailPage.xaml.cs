using ControlPhoneMaster.ViewModels;
using System.ComponentModel;
using Xamarin.Forms;

namespace ControlPhoneMaster.Views
{
    public partial class ItemDetailPage : ContentPage
    {
        public ItemDetailPage()
        {
            InitializeComponent();
            BindingContext = new ItemDetailViewModel();
        }
    }
}