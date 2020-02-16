<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <TABLE BORDER="1">
      <TR>
        <TD>��</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">last_name</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/���O/��"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>���t���K�i</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">last_name_yomi</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/���O/���t���K�i"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>��</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">first_name</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/���O/��"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>���t���K�i</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">first_name_yomi</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/���O/���t���K�i"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>����</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">radio</xsl:attribute>
            <xsl:attribute name="NAME">sex</xsl:attribute>
            <xsl:attribute name="VALUE">male</xsl:attribute>
            <xsl:if test="�ڋq/����='male'">
              <xsl:attribute name="CHECKED">true</xsl:attribute>
            </xsl:if>
          </xsl:element>
          <xsl:text>�j</xsl:text>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">radio</xsl:attribute>
            <xsl:attribute name="NAME">sex</xsl:attribute>
            <xsl:attribute name="VALUE">female</xsl:attribute>
            <xsl:if test="�ڋq/����='female'">
              <xsl:attribute name="CHECKED">true</xsl:attribute>
            </xsl:if>
          </xsl:element>
          <xsl:text>��</xsl:text>
        </TD>
      </TR>
      <TR>
        <TD>���N����</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">4</xsl:attribute>
            <xsl:attribute name="NAME">birth_year</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/���N����/�N"/></xsl:attribute>
          </xsl:element>
          <xsl:text>�N</xsl:text>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">2</xsl:attribute>
            <xsl:attribute name="NAME">birth_month</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/���N����/��"/></xsl:attribute>
          </xsl:element>
          <xsl:text>��</xsl:text>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">2</xsl:attribute>
            <xsl:attribute name="NAME">birth_day</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/���N����/��"/></xsl:attribute>
          </xsl:element>
          <xsl:text>��</xsl:text>
        </TD>
      </TR>
      <TR>
        <TD>�X�֔ԍ�</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">7</xsl:attribute>
            <xsl:attribute name="NAME">zip_code</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/�Z��/�X�֔ԍ�"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>�s���{��</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">8</xsl:attribute>
            <xsl:attribute name="NAME">prifecture</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/�Z��/�s���{��"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>�s�����Ԓn�P</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">address1</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/�Z��/�s�����Ԓn�P"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>�s�����Ԓn�Q</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">address2</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/�Z��/�s�����Ԓn�Q"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>�s�����Ԓn�R</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">address3</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/�Z��/�s�����Ԓn�R"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>�d�b</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">phone</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/�d�b�ԍ�"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>�g�ѓd�b</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">mobile</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/�g�ѓd�b"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>�d�q���[��</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">hidden</xsl:attribute>
            <xsl:attribute name="NAME">email</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/�d�q���[��"/></xsl:attribute>
          </xsl:element>
          <xsl:text><xsl:value-of select="�ڋq/�d�q���[��"/></xsl:text>
        </TD>
      </TR>
      <TR>
        <TD>�E��</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">job</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/�E��"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>�Ζ���</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">company</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/�Ζ���"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>�����P</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">interest1</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/����/�����P"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>�����Q</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">interest2</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/����/�����Q"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
      <TR>
        <TD>�����R</TD>
        <TD>
          <xsl:element name="INPUT">
            <xsl:attribute name="TYPE">text</xsl:attribute>
            <xsl:attribute name="SIZE">23</xsl:attribute>
            <xsl:attribute name="NAME">interest3</xsl:attribute>
            <xsl:attribute name="VALUE"><xsl:value-of select="�ڋq/����/�����R"/></xsl:attribute>
          </xsl:element>
        </TD>
      </TR>
    </TABLE>
  </xsl:template> 

</xsl:stylesheet>
