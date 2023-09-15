using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace UProtectApp
{

    internal class KprotectDll
    {
        /// Return Type: boolean
        [System.Runtime.InteropServices.DllImportAttribute("KProtectDll.dll", EntryPoint = "InitKProtectInteface")]
        [return: System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.I1)]
        public static extern bool InitKProtectInteface();


        /// Return Type: boolean
        ///Path: PWCHAR->WCHAR*
        [System.Runtime.InteropServices.DllImportAttribute("KProtectDll.dll", EntryPoint = "AddRegistryPathToProtect", CharSet =CharSet.Unicode)]
        [return: System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.I1)]
        public static extern bool AddRegistryPathToProtect(string Path);


        /// Return Type: boolean
        [System.Runtime.InteropServices.DllImportAttribute("KProtectDll.dll", EntryPoint = "ApplyRegistryPaths")]
        [return: System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.I1)]
        public static extern bool ApplyRegistryPaths();


        /// Return Type: boolean
        ///Path: PWCHAR->WCHAR*
        [System.Runtime.InteropServices.DllImportAttribute("KProtectDll.dll", EntryPoint = "AddFilePathToProtect", CharSet = CharSet.Unicode)]
        [return: System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.I1)]
        public static extern bool AddFilePathToProtect(string Path);


        /// Return Type: boolean
        [System.Runtime.InteropServices.DllImportAttribute("KProtectDll.dll", EntryPoint = "ApplyFilePaths")]
        [return: System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.I1)]
        public static extern bool ApplyFilePaths();

        /// Return Type: boolean
        ///Paths: PWCHAR->WCHAR*
        ///Size: int
        [System.Runtime.InteropServices.DllImportAttribute("KProtectDll.dll", EntryPoint = "QueryReg")]
        [return: System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.I1)]
        public static extern bool QueryReg(System.IntPtr Paths, int Size);



        /// Return Type: boolean
        ///Paths: PWCHAR->WCHAR*
        ///Size: int
        [System.Runtime.InteropServices.DllImportAttribute("KProtectDll.dll", EntryPoint = "QueryMiniFilter")]
        [return: System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.I1)]
        public static extern bool QueryMiniFilter(System.IntPtr Paths, int Size);

        /// Return Type: boolean
        ///Path: PWCHAR->WCHAR*
        [System.Runtime.InteropServices.DllImportAttribute("KProtectDll.dll", EntryPoint = "RemoveRegPath", CharSet = CharSet.Unicode)]
        [return: System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.I1)]
        public static extern bool RemoveRegPath(string Path);


        /// Return Type: boolean
        ///Path: PWCHAR->WCHAR*
        [System.Runtime.InteropServices.DllImportAttribute("KProtectDll.dll", EntryPoint = "RemoveMiniFilterPath", CharSet = CharSet.Unicode)]
        [return: System.Runtime.InteropServices.MarshalAsAttribute(System.Runtime.InteropServices.UnmanagedType.I1)]
        public static extern bool RemoveMiniFilterPath(string Path);


    }
}
