<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="xml" encoding="UTF-8" />

  <xsl:template match="/">
    <xsl:element name="SELECT">
      <xsl:attribute name="NAME">to_year</xsl:attribute>
      <xsl:attribute name="SIZE">1</xsl:attribute>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2002'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2002</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2003'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2003</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2004'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2004</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2005'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2005</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2006'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2006</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2007'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2007</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2008'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2008</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2009'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2009</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2010'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2010</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2011'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2011</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2012'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2012</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2013'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2013</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2014'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2014</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2015'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2015</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2016'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2016</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2017'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2017</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2018'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2018</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2019'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2019</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2020'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2020</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Year='2021'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>2021</xsl:text>
      </xsl:element>
    </xsl:element>
    <xsl:text>”N</xsl:text>
    <xsl:element name="SELECT">
      <xsl:attribute name="NAME">to_month</xsl:attribute>
      <xsl:attribute name="SIZE">1</xsl:attribute>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Month='1'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>01</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Month='2'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>02</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Month='3'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>03</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Month='4'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>04</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Month='5'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>05</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Month='6'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>06</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Month='7'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>07</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Month='8'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>08</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Month='9'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>09</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Month='10'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>10</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Month='11'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>11</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Month='12'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>12</xsl:text>
      </xsl:element>
    </xsl:element>
    <xsl:text>Œ</xsl:text>
    <xsl:element name="SELECT">
      <xsl:attribute name="NAME">to_day</xsl:attribute>
      <xsl:attribute name="SIZE">1</xsl:attribute>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='1'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>01</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='2'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>02</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='3'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>03</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='4'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>04</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='5'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>05</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='6'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>06</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='7'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>07</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='8'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>08</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='9'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>09</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='10'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>10</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='11'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>11</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='12'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>12</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='13'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>13</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='14'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>14</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='15'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>15</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='16'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>16</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='17'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>17</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='18'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>18</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='19'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>19</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='20'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>20</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='21'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>21</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='22'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>22</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='23'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>23</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='24'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>24</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='25'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>25</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='26'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>26</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='27'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>27</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='28'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>28</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='29'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>29</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='30'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>30</xsl:text>
      </xsl:element>
      <xsl:element name="OPTION">
        <xsl:if test="Date/Day='31'">
          <xsl:attribute name="SELECTED">true</xsl:attribute>
        </xsl:if>
        <xsl:text>31</xsl:text>
      </xsl:element>
    </xsl:element>
    <xsl:text>“ú</xsl:text>
  </xsl:template> 

</xsl:stylesheet>
