using System;
using System.Collections.Generic;
using System.Linq;

class Window
{
    public int TotalTime { get; set; }
    public List<string> Tickets { get; } = new List<string>();
}

class Program
{
    static void Main()
    {
        Console.WriteLine("Введите кол-во окон");
        int windowsCount = int.Parse(Console.ReadLine()!);

        List<(int time, string ticket)> visitors = new List<(int, string)>();
        int ticketCounter = 0;

        while (true)
        {
            string input = Console.ReadLine()!.Trim();
            if (string.IsNullOrEmpty(input)) continue;

            string[] parts = input.Split(' ');
            string command = parts[0].ToUpper();

            if (command == "ENQUEUE")
            {
                int time = int.Parse(parts[1]);
                string ticket = $"T{ticketCounter:D3}";
                ticketCounter++;
                visitors.Add((time, ticket));
                Console.WriteLine($"{ticket}");
            }
            else if (command == "DISTRIBUTE")
            {
                break;
            }
            else
            {
                Console.WriteLine("Неизвестная команда");
                return;
            }
        }

        Window[] windows = new Window[windowsCount];
        for (int i = 0; i < windowsCount; i++)
            windows[i] = new Window();

        foreach (var visitor in visitors)
        {
            Window targetWindow = windows
                .OrderBy(w => w.TotalTime)
                .ThenBy(w => Array.IndexOf(windows, w))
                .First();

            targetWindow.TotalTime += visitor.time;
            targetWindow.Tickets.Add(visitor.ticket);
        }

        for (int i = 0; i < windows.Length; i++)
        {
            Console.WriteLine($"Окно {i + 1} ({windows[i].TotalTime} минут): " +
                               string.Join(", ", windows[i].Tickets));
        }
    }
}
