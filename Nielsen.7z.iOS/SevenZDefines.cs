// -----------------------------------------------------------------------
//  <copyright file="SevenZDefines.cs" company="Nielsen">
//      Copyright (c) Nielsen. All rights reserved.
//  </copyright>
// -----------------------------------------------------------------------
namespace Nielsen.SevenZ
{
    /// <summary>
    /// Seven Z Archiver defines class for iOS.
    /// </summary>
    public static class SevenZDefines
    {
        /// <summary>
        /// Archiver Operetion result
        /// </summary>
        public enum SevenZResult: int
        {
            Success = 0,
            Warning,
            FatalError,
            UserError = 7,
            MemoryError,
            UserBreak = 255
        }

        /// <summary>
        /// Archiver Compression Level
        /// </summary>
        public enum CompressionLevel : int
        {
            mx0, // Don't compress at all - just copy the contents to archive.
            mx1, // Consumes least time, but compression is low.
            mx3, // Better than -mx1.
            mx5, // This is default(compression is normal).
            mx7, // Maximum compression.
            mx9, // Ultra compression.
        }
    }
}