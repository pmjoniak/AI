using mshtml;
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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace connect4_web
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		private static DispatcherTimer zombie = new DispatcherTimer();
		private static DispatcherTimer necromancer = new DispatcherTimer();

		int[] tab = new int[7]{0,0,0,0,0,0,0};

		public MainWindow()
		{
			InitializeComponent();
			this.Loaded += MainWindow_Loaded;
			zombie.Tick += t_Tick;
			zombie.Interval = new TimeSpan(0, 0, 0, 0, 300);
			zombie.IsEnabled = false;
			necromancer.IsEnabled = false;
			necromancer.Interval = new TimeSpan(0, 0, 0, 0, 700);
			necromancer.Tick += necromancer_Tick;
		}

		void necromancer_Tick(object sender, EventArgs e)
		{
			zombie.IsEnabled = true;
			necromancer.IsEnabled = false;
		}

		void t_Tick(object sender, EventArgs e)
		{
			var document = (IHTMLDocument3)web_browser.Document;

			var last = document.getElementsByName("last_dropIt")
						.OfType<IHTMLElement>()
						.Select(element => element.getAttribute("value"))
						.FirstOrDefault();

			var value =	document.getElementsByName("texter")
						.OfType<IHTMLElement>()
						.Select(element => element.getAttribute("value"))
						.FirstOrDefault();
			if (last != null && last != "")
			{
				int info = int.Parse(last);
				if (info == 8)
					return;
				if(info == 7)
					for(int i = 0; i < 7; i++)
						tab[i] = 0;
				else
					tab[info]++;
			}

			if(value == "User's turn.")
			{
				Random r = new Random();
				int idx = r.Next(0, 6);
				while(tab[idx] >= 6) idx = r.Next(0, 7);
				
				web_browser.InvokeScript("dropIt", idx);
				tab[idx]++;
				zombie.IsEnabled = false;
				necromancer.IsEnabled = true;
			}
		}

		void MainWindow_Loaded(object sender, RoutedEventArgs e)
		{
			web_browser.Navigate("C:/Users/Pawel/Desktop/gra/iconnect4.html");
			web_browser.Navigated += web_browser_Navigated;
		}

		void web_browser_Navigated(object sender, NavigationEventArgs e)
		{
			zombie.IsEnabled = true;

		}
	}
}
