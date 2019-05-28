<?xml version='1.0' encoding='ISO-8859-1' standalone='yes'?>
<tagfile>
  <compound kind="page">
    <filename>index</filename>
    <title></title>
    <name>index</name>
    <docanchor>Variants</docanchor>
    <docanchor>readwrite</docanchor>
    <docanchor>interrupts</docanchor>
    <docanchor>General</docanchor>
    <docanchor>FuncDesc</docanchor>
  </compound>
  <compound kind="file">
    <name>z82_doc.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/Z082/DRIVER/COM/</path>
    <filename>z82__doc_8c</filename>
  </compound>
  <compound kind="file">
    <name>z82_drv.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/Z082/DRIVER/COM/</path>
    <filename>z82__drv_8c</filename>
    <class kind="struct">LL_HANDLE</class>
    <member kind="define">
      <type>#define</type>
      <name>_NO_LL_HANDLE</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CH_NUMBER</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>USE_IRQ</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ADDRSPACE_COUNT</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ADDRSPACE_SIZE</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DBG_MYLEVEL</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DBH</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>OSH</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z82_REG_RESULT</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z82_REG_STATUS</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z82_REG_DEBOUNCER</name>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z82_DEBOUNCE_MASK</name>
      <anchor>a11</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z82_DEBOUNCE_DEFAULT</name>
      <anchor>a12</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z82_Init</name>
      <anchor>a13</anchor>
      <arglist>(DESC_SPEC *descSpec, OSS_HANDLE *osHdl, MACCESS *ma, OSS_SEM_HANDLE *devSemHdl, OSS_IRQ_HANDLE *irqHdl, LL_HANDLE **llHdlP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z82_Exit</name>
      <anchor>a14</anchor>
      <arglist>(LL_HANDLE **llHdlP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z82_Read</name>
      <anchor>a15</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 *value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z82_Write</name>
      <anchor>a16</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z82_SetStat</name>
      <anchor>a17</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 code, INT32_OR_64 value32_or_64)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z82_GetStat</name>
      <anchor>a18</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 code, INT32_OR_64 *value32_or_64P)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z82_BlockRead</name>
      <anchor>a19</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size, int32 *nbrRdBytesP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z82_BlockWrite</name>
      <anchor>a20</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size, int32 *nbrWrBytesP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z82_Irq</name>
      <anchor>a21</anchor>
      <arglist>(LL_HANDLE *llHdl)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Z82_Info</name>
      <anchor>a22</anchor>
      <arglist>(int32 infoType,...)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>char *</type>
      <name>Ident</name>
      <anchor>a23</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Cleanup</name>
      <anchor>a24</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 retCode)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>__Z82_GetEntry</name>
      <anchor>a25</anchor>
      <arglist>(LL_ENTRY *drvP)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>z82_drv.h</name>
    <path>/opt/menlinux/INCLUDE/COM/MEN/</path>
    <filename>z82__drv_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>Z82_CLR_RESULT</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z82_GET_RESULT</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z82_GET_OVF</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z82_DEBOUNCE</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z82_VARIANT</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_Z82_GLOBNAME</name>
      <anchor>a5</anchor>
      <arglist>(var, name)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Z82_GLOBNAME</name>
      <anchor>a6</anchor>
      <arglist>(var, name)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>__Z82_GetEntry</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT32_OR_64</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>U_INT32_OR_64</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>INT32_OR_64</type>
      <name>MDIS_PATH</name>
      <anchor>a10</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>z82_simp.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/Z082/EXAMPLE/Z82_SIMP/COM/</path>
    <filename>z82__simp_8c</filename>
    <member kind="function" static="yes">
      <type>void</type>
      <name>PrintError</name>
      <anchor>a0</anchor>
      <arglist>(char *info)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>main</name>
      <anchor>a1</anchor>
      <arglist>(int argc, char *argv[])</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>LL_HANDLE</name>
    <filename>structLL__HANDLE.html</filename>
    <member kind="variable">
      <type>int32</type>
      <name>memAlloc</name>
      <anchor>m0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OSS_HANDLE *</type>
      <name>osHdl</name>
      <anchor>m1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OSS_IRQ_HANDLE *</type>
      <name>irqHdl</name>
      <anchor>m2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DESC_HANDLE *</type>
      <name>descHdl</name>
      <anchor>m3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MACCESS</type>
      <name>ma</name>
      <anchor>m4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MDIS_IDENT_FUNCT_TBL</type>
      <name>idFuncTbl</name>
      <anchor>m5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>dbgLevel</name>
      <anchor>m6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DBG_HANDLE *</type>
      <name>dbgHdl</name>
      <anchor>m7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>nDebounce</name>
      <anchor>m8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>z82dummy</name>
    <title>MEN logo</title>
    <filename>z82dummy</filename>
  </compound>
</tagfile>
