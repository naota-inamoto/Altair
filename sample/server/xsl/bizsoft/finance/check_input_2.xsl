<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <xsl:apply-templates select="EBIZ"/>
  </xsl:template> 

  <xsl:template match="EBIZ">
      <xsl:element name="INPUT">
        <xsl:attribute name="TYPE">hidden</xsl:attribute>
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_selected_ah</xsl:attribute>
        <xsl:attribute name="VALUE"><xsl:value-of select="Selected"/></xsl:attribute>
      </xsl:element>
      <xsl:value-of select="Selected"/>
      <xsl:variable name="selected" select="Selected2"/>
      <xsl:element name="SELECT">
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_selected_as</xsl:attribute>
        <xsl:for-each select="����Ȗ�/�⏕�Ȗ�">
          <xsl:element name="OPTION">
            <xsl:if test="����=$selected">
              <xsl:attribute name="SELECTED">true</xsl:attribute>
            </xsl:if>
            <xsl:value-of select="����"/>
          </xsl:element>
        </xsl:for-each>
      </xsl:element>
      <xsl:text>���z</xsl:text>
      <xsl:element name="INPUT">
        <xsl:attribute name="TYPE">text</xsl:attribute>
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_money</xsl:attribute>
        <xsl:attribute name="SIZE">12</xsl:attribute>
        <xsl:attribute name="VALUE"><xsl:value-of select="����Ȗ�/���z"/></xsl:attribute>
      </xsl:element>
      <BR/>
      <xsl:element name="INPUT">
        <xsl:attribute name="TYPE">hidden</xsl:attribute>
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax_kind</xsl:attribute>
        <xsl:attribute name="VALUE"><xsl:value-of select="����Ȗ�/�ŋ敪"/></xsl:attribute>
      </xsl:element>
      <xsl:text>�ŋ敪�F</xsl:text>
      <xsl:value-of select="����Ȗ�/�ŋ敪"/>
      <xsl:text>&nbsp;</xsl:text>
      <xsl:if test="����Ȗ�/�ŋ敪='�ېŎd��'">
        <xsl:text>�Ŋz</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax</xsl:attribute>
          <xsl:attribute name="SIZE">12</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="����Ȗ�/�Ŋz"/></xsl:attribute>
        </xsl:element>
        <xsl:text>�ŗ�</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax_rate</xsl:attribute>
          <xsl:attribute name="SIZE">3</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="����Ȗ�/�ŗ�"/></xsl:attribute>
        </xsl:element>
      </xsl:if>
      <xsl:if test="����Ȗ�/�ŋ敪='�ېŎd���Ԋ�'">
        <xsl:text>�Ŋz</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax</xsl:attribute>
          <xsl:attribute name="SIZE">12</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="����Ȗ�/�Ŋz"/></xsl:attribute>
        </xsl:element>
        <xsl:text>�ŗ�</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax_rate</xsl:attribute>
          <xsl:attribute name="SIZE">3</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="����Ȗ�/�ŗ�"/></xsl:attribute>
        </xsl:element>
      </xsl:if>
      <xsl:if test="����Ȗ�/�ŋ敪='�ېŔ���'">
        <xsl:text>�Ŋz</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax</xsl:attribute>
          <xsl:attribute name="SIZE">12</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="����Ȗ�/�Ŋz"/></xsl:attribute>
        </xsl:element>
        <xsl:text>�ŗ�</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax_rate</xsl:attribute>
          <xsl:attribute name="SIZE">3</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="����Ȗ�/�ŗ�"/></xsl:attribute>
        </xsl:element>
      </xsl:if>
      <xsl:if test="����Ȗ�/�ŋ敪='�ېŔ���Ԋ�'">
        <xsl:text>�Ŋz</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax</xsl:attribute>
          <xsl:attribute name="SIZE">12</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="����Ȗ�/�Ŋz"/></xsl:attribute>
        </xsl:element>
        <xsl:text>�ŗ�</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax_rate</xsl:attribute>
          <xsl:attribute name="SIZE">3</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="����Ȗ�/�ŗ�"/></xsl:attribute>
        </xsl:element>
      </xsl:if>
      <xsl:if test="����Ȗ�/�ŋ敪='�ېőݓ|'">
        <xsl:text>�Ŋz</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax</xsl:attribute>
          <xsl:attribute name="SIZE">12</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="����Ȗ�/�Ŋz"/></xsl:attribute>
        </xsl:element>
        <xsl:text>�ŗ�</xsl:text>
        <xsl:element name="INPUT">
          <xsl:attribute name="TYPE">text</xsl:attribute>
          <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tax_rate</xsl:attribute>
          <xsl:attribute name="SIZE">3</xsl:attribute>
          <xsl:attribute name="VALUE"><xsl:value-of select="����Ȗ�/�ŗ�"/></xsl:attribute>
        </xsl:element>
      </xsl:if>
      <BR/>
      <xsl:text>�E�v</xsl:text>
      <xsl:element name="INPUT">
        <xsl:attribute name="TYPE">text</xsl:attribute>
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_summary</xsl:attribute>
        <xsl:attribute name="SIZE">50</xsl:attribute>
        <xsl:attribute name="VALUE"><xsl:value-of select="����Ȗ�/�E�v"/></xsl:attribute>
      </xsl:element>
      <BR/>
      <xsl:text>����</xsl:text>
      <xsl:element name="INPUT">
        <xsl:attribute name="TYPE">text</xsl:attribute>
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_memo</xsl:attribute>
        <xsl:attribute name="SIZE">50</xsl:attribute>
        <xsl:attribute name="VALUE"><xsl:value-of select="����Ȗ�/����"/></xsl:attribute>
      </xsl:element>
      <BR/>
      <xsl:text>�t�</xsl:text>
      <xsl:element name="SELECT">
        <xsl:attribute name="NAME"><xsl:value-of select="Name"/>_tag</xsl:attribute>
        <xsl:element name="OPTION">
          <xsl:if test="�t�='�L'">
            <xsl:attribute name="SELECTED">true</xsl:attribute>
          </xsl:if>
          <xsl:text>�L</xsl:text>
        </xsl:element>
        <xsl:element name="OPTION">
          <xsl:if test="�t�='��'">
            <xsl:attribute name="SELECTED">true</xsl:attribute>
          </xsl:if>
          <xsl:text>��</xsl:text>
        </xsl:element>
      </xsl:element>
      <BR/>
  </xsl:template> 

</xsl:stylesheet>
