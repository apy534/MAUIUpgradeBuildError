// -----------------------------------------------------------------------
//  <copyright file="SevenZImpl.cs" company="Nielsen">
//      Copyright (c) Nielsen. All rights reserved.
//  </copyright>
// -----------------------------------------------------------------------
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using ObjCRuntime;
using static Nielsen.SevenZ.SevenZDefines;

namespace Nielsen.SevenZ
{
    /// <summary>
    /// Seven Z Archiver impl class for iOS.
    /// </summary>
    public static class SevenZImpl
    {
        // You must flag all of your callback methods with the MonoPInvokeCallbackAttribute
        // The methods have to be static methods, there is no support for instance methods.
        private delegate void FProgressPtr(ulong completed, ulong total);
        static IProgress<(ulong completed, ulong total)> Progress;
        [MonoPInvokeCallback(typeof(FProgressPtr))]
        private static void FProgressCallbackInternal(ulong completed, ulong total)
        {
            Progress?.Report((completed, total));
        }


        [StructLayout(LayoutKind.Sequential)]
        private class ArchiveInfo
        {
            public ulong totalPackSize;
            public ulong totalUnpackSize;
        }

        #region My7za methods
        [DllImport("__Internal", EntryPoint = "external_main", CallingConvention = CallingConvention.Cdecl)]
        private static extern SevenZResult Main(
            int numArgs,
            [MarshalAsAttribute(UnmanagedType.LPArray, ArraySubType = UnmanagedType.LPStr)] string[] str,
            FProgressPtr progressPtr,
            ArchiveInfo outArchiveInfo);

        [DllImport("__Internal", EntryPoint = "external_BreakExecution", CallingConvention = CallingConvention.Cdecl)]
        private static extern void BreakExecution();
        #endregion

        public static SevenZResult Encode(
            IEnumerable<string> filesToArchivePath, string archiveFilePath, string workDir, bool zip,
            CompressionLevel compressionLevel, string secretKey, IProgress<(ulong completed, ulong total)> progress,
            IEnumerable<string> filesToExclude)
        {
            var list = new List<string>
            {
                "7za",
                "a"
            };

            if (zip)
            {
                list.Add("-tzip");
            }

            switch (compressionLevel)
            {
                case CompressionLevel.mx0:
                    list.Add("-mx0");
                    break;
                case CompressionLevel.mx1:
                    list.Add("-mx1");
                    break;
                case CompressionLevel.mx3:
                    list.Add("-mx3");
                    break;
                case CompressionLevel.mx5:
                    list.Add("-mx5");
                    break;
                case CompressionLevel.mx7:
                    list.Add("-mx7");
                    break;
                case CompressionLevel.mx9:
                    list.Add("-mx9");
                    break;
            }

            list.Add(string.Format("-w{0}", workDir));

            if (!string.IsNullOrEmpty(secretKey))
            {
                list.Add($"-p{secretKey}");

                if (!zip)
                {
                    //-mhe header encryption FOR .7z ONLY, i.e. file names inside archive
                    // will not be seen used together with password option
                    list.Add(string.Format("-mhe"));
                }
            }

            list.Add(archiveFilePath);
            list.AddRange(filesToArchivePath);

            foreach (var excludeFile in filesToExclude)
            {
                list.Add($"-x!{excludeFile}");
            }

            Progress = progress;
            var res = Main(list.Count, list.ToArray(), FProgressCallbackInternal, new ArchiveInfo());
            Progress = null;
            return res;
        }

        public static SevenZResult Encode(
            string fileToArchivePath, string archiveFilePath, string workDir, bool zip,
            CompressionLevel compressionLevel, string secretKey, IProgress<(ulong completed, ulong total)> progress)
        {
            return Encode(new List<string> { fileToArchivePath }, archiveFilePath, workDir, zip, compressionLevel, secretKey, progress, new List<string>());
        }

        public static SevenZResult Decode(string archiveFilePath, string destinationDirectory, string secretKey, IProgress<(ulong completed, ulong total)> progress)
        {
            var list = new List<string>
            {
                "7za",
                "x"
            };

            if (!string.IsNullOrEmpty(secretKey))
            {
                list.Add($"-p{secretKey}");
            }

            list.Add(archiveFilePath);

            list.Add(string.Format("-o{0}", destinationDirectory));
            list.Add("-y");

            Progress = progress;
            var res = Main(list.Count, list.ToArray(), FProgressCallbackInternal, new ArchiveInfo());
            Progress = null;
            return res;
        }

        public static ulong? GetArchiveUnpackSize(string archiveFilePath, string secretKey)
        {
            var list = new List<string>
            {
                "7za",
                "l"
            };

            if (!string.IsNullOrEmpty(secretKey))
            {
                list.Add($"-p{secretKey}");
            }

            list.Add(archiveFilePath);

            ArchiveInfo archiveInfo = new ArchiveInfo();
            var ret = Main(list.Count, list.ToArray(), null, archiveInfo);
            return ret == SevenZResult.Success ? archiveInfo.totalUnpackSize : (ulong?)null;
        }

        public static void BreakSevenZExecution()
        {
            BreakExecution();
        }
    }
}