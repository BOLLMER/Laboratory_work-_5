using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace WarehouseApp
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.OutputEncoding = Encoding.UTF8;
            Warehouse warehouse = new Warehouse();
            Console.WriteLine("Складская система готова к работе. Команды: ADD, REMOVE, INFO");

            while (true)
            {
                try
                {
                    Console.Write("> ");
                    string[] command = Console.ReadLine().Split(new[] { ' ' }, 4, StringSplitOptions.RemoveEmptyEntries);

                    if (command.Length == 0) continue;

                    switch (command[0].ToUpper())
                    {
                        case "ADD" when command.Length == 4:
                            warehouse.AddProduct(command[1], int.Parse(command[2]), command[3]);
                            break;

                        case "REMOVE" when command.Length == 4:
                            warehouse.RemoveProduct(command[1], int.Parse(command[2]), command[3]);
                            break;

                        case "INFO":
                            warehouse.GetInfo();
                            break;

                        default:
                            Console.WriteLine("Неизвестная команда или неверный формат");
                            break;
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Ошибка: {ex.Message}");
                }
            }
        }
    }

    class Warehouse
    {
        private readonly Dictionary<string, Dictionary<string, int>> _cells = new();
        private const int TotalCapacity = 34650;

        public bool IsValidAddress(string address)
        {
            return Regex.IsMatch(address, @"^[A-G](0[1-9]|1[0-5])(0[1-9]|1[0-1])[1-3]$");
        }

        public void AddProduct(string product, int quantity, string address)
        {
            if (!IsValidAddress(address))
                throw new ArgumentException("Некорректный адрес ячейки");

            if (!_cells.TryGetValue(address, out var cell))
            {
                cell = new Dictionary<string, int>();
                _cells[address] = cell;
            }

            int currentTotal = GetTotalQuantity(cell);
            if (currentTotal + quantity > 10)
                throw new InvalidOperationException("Превышена вместимость ячейки (макс. 10 единиц)");

            cell[product] = cell.TryGetValue(product, out int current) ? current + quantity : quantity;
            Console.WriteLine($"Добавлено {quantity} единиц товара «{product}» в ячейку {address}");
        }

        public void RemoveProduct(string product, int quantity, string address)
        {
            if (!IsValidAddress(address))
                throw new ArgumentException("Некорректный адрес ячейки");

            if (!_cells.TryGetValue(address, out var cell) || !cell.ContainsKey(product))
                throw new KeyNotFoundException("Товар отсутствует в ячейке");

            if (cell[product] < quantity)
                throw new InvalidOperationException("Недостаточно товара для списания");

            cell[product] -= quantity;
            if (cell[product] == 0) cell.Remove(product);
            if (cell.Count == 0) _cells.Remove(address);

            Console.WriteLine($"Удалено {quantity} единиц товара «{product}» из ячейки {address}");
        }

        public void GetInfo()
        {
            int totalUsed = 0;
            var zoneUsage = new Dictionary<char, int>();

            foreach (var (address, cell) in _cells)
            {
                int cellTotal = GetTotalQuantity(cell);
                totalUsed += cellTotal;
                char zone = address[0];
                zoneUsage[zone] = zoneUsage.GetValueOrDefault(zone) + cellTotal;
            }

            double totalPercent = (double)totalUsed / TotalCapacity * 100;
            Console.WriteLine($"Общая загруженность склада: {totalPercent:F2}%");

            Console.WriteLine("\nЗагруженность по зонам:");
            int zoneCapacity = TotalCapacity / 7;
            for (char zone = 'A'; zone <= 'G'; zone++)
            {
                double percent = (double)zoneUsage.GetValueOrDefault(zone) / zoneCapacity * 100;
                Console.WriteLine($"  Зона {zone}: {percent:F2}%");
            }

            Console.WriteLine("\nСодержимое занятых ячеек:");
            foreach (var address in _cells.Keys)
            {
                var products = string.Join(", ", _cells[address].Select(kvp => $"{kvp.Key} ({kvp.Value})"));
                Console.WriteLine($"  {address}: {products}");
            }

            Console.WriteLine("\nПустые ячейки (первые 10):");
        }

        private static int GetTotalQuantity(Dictionary<string, int> cell) => cell.Values.Sum();
    }
}
