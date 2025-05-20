using System;
using System.Collections.Generic;
using System.Linq;

public enum Command
{
    CREATE_TRAM,
    TRAMS_IN_STOP,
    STOPS_IN_TRAM,
    TRAMS,
    UNKNOWN
}
public static class Data
{
    public static Dictionary<string, List<string>> Trams = new Dictionary<string, List<string>>();

    public static Dictionary<string, HashSet<string>> Stops = new Dictionary<string, HashSet<string>>();

    public static void AddTram(string tram, List<string> newStops)
    {
        Trams[tram] = newStops;
        foreach (var stop in newStops)
        {
            if (!Stops.ContainsKey(stop))
                Stops[stop] = new HashSet<string>();
            Stops[stop].Add(tram);
        }
    }

    public static void PrintTramsForStop(string stop)
    {
        if (!Stops.ContainsKey(stop) || Stops[stop].Count == 0)
        {
            Console.WriteLine("Trans is absent");
            return;
        }
        Console.WriteLine(string.Join(" ", Stops[stop]));
    }

    public static void PrintStopsForTram(string tram)
    {
        if (!Trams.ContainsKey(tram))
        {
            Console.WriteLine("Stops is absent");
            return;
        }
        foreach (var stop in Trams[tram])
        {
            var otherTrams = Stops.GetValueOrDefault(stop, new HashSet<string>())
                .Where(t => t != tram)
                .ToList();

            Console.WriteLine($"Stop {stop}: {(otherTrams.Any() ? string.Join(" ", otherTrams) : "0")}");
        }
    }

    public static void PrintAllTrams()
    {
        if (!Trams.Any())
        {
            Console.WriteLine("Trans is absent");
            return;
        }
        foreach (var tram in Trams)
        {
            Console.WriteLine($"TRAM {tram.Key}: {string.Join(" ", tram.Value)}");
        }
    }
}

public class Program
{
    private static Command ParseCommand(string input)
    {
        return input switch
        {
            "CREATE_TRAM" => Command.CREATE_TRAM,
            "TRAMS_IN_STOP" => Command.TRAMS_IN_STOP,
            "STOPS_IN_TRAM" => Command.STOPS_IN_TRAM,
            "TRAMS" => Command.TRAMS,
            _ => Command.UNKNOWN  
        };
    }

    private static void ExecuteCommand(Command cmd, List<string> args)
    {
        switch (cmd)
        {
            case Command.CREATE_TRAM:
                if (args.Count < 2)
                {
                    Console.WriteLine("Error: Not enough arguments");
                    return;
                }
                Data.AddTram(args[0], args.Skip(1).ToList());
                break;
            case Command.TRAMS_IN_STOP:
                if (args.Count == 0)
                {
                    Console.WriteLine("Error: Missing stop name");
                    return;
                }
                Data.PrintTramsForStop(args[0]);
                break;
            case Command.STOPS_IN_TRAM:
                if (args.Count == 0)
                {
                    Console.WriteLine("Error: Missing tram name");
                    return;
                }
                Data.PrintStopsForTram(args[0]);
                break;
            case Command.TRAMS:
                Data.PrintAllTrams();
                break;
            default:
                Console.WriteLine("Unknown command");
                break;
        }
    }

    public static void Main()
    {
        string line;
        while ((line = Console.ReadLine()) != null)
        {
            var tokens = line.Trim()
                .Split(new[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
            if (tokens.Length == 0) continue;

            var cmd = ParseCommand(tokens[0]);
            var args = tokens.Skip(1).ToList();
            ExecuteCommand(cmd, args);
        }
    }
}