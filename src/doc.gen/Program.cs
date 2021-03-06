﻿/*
 * Created by: egr
 * Created at: 19.10.2010
 * © 2009-2020 Alexander Egorov
 */

using System.Collections.Generic;
using System.IO;
using System.Text;
using Antlr3.ST;

namespace doc.gen
{
    internal static class Program
    {
        private static readonly IDictionary<string, string> hcNames = new Dictionary<string, string>
        {
            { "en", "Hash Calculator" },
            { "ru", "Хэш калькулятор" }
        };

        private static void CreateHqDocumentationTxt(string docPath, string template, string lang)
        {
            var stringTemplate = new StringTemplate(File.ReadAllText(Path.Combine(docPath, template)));
            stringTemplate.SetAttribute("langName", hcNames[lang]);
            stringTemplate.SetAttribute("appName", "hc");
            File.WriteAllText(Path.Combine(docPath, @"Readme.hc." + lang + ".txt"), stringTemplate.ToString(),
                Encoding.UTF8);
        }

        private static void Main(string[] args)
        {
            if (args.Length == 0)
                return;
            var docPath = args[0];

            CreateHqDocumentationTxt(docPath, @"Readme.hc.ru.st", "ru");
            CreateHqDocumentationTxt(docPath, @"Readme.hc.en.st", "en");
        }
    }
}