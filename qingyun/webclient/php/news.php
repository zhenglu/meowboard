<?php header('Content-Type: text/xml encoding="utf-8"');?>

<?php
require_once("connection.php");

class News extends db{


	function makeNews($result){
		
		$xml_ret = "\n<rss version='2.0'>\n<channel>";
		$xml_ret .= "\n\t<description></description>";
		$xml_ret .= "\n\t<link>http://xadrezlivre.c3sl.ufpr.br</link>";
		$xml_ret .= "\n\t<title>Xadrez Livre - Notícias</title>";
		$xml_ret .= "\n\t<language>pt-br</language>";
		
		while($row=pg_fetch_row($result, $i)){
			$xml_ret .= "\n\t<item about='www.c3sl.ufpr.br'>";
			$xml_ret .= "\n\t\t<title>$row[1]</title>";
			$xml_ret .= "\n\t\t<link>www.c3sl.ufpr.br</link>";
			$xml_ret .= "\n\t\t<description><![CDATA[$row[2]]]></description>";
			$xml_ret .= "\n\t\t<pubDate>$row[0]</pubDate>";
			$xml_ret .= "\n\t\t<author>$row[3]</author>";

			$xml_ret .= "\n\t</item>";
		}
		
		$xml_ret .= "\n</channel>\n</rss>";

		return $xml_ret;

	}
	
	function selectNews(){
		$query = "select to_char(now(), 'Dy, D Mon YYYY HH:MM:ss GMT') as news_data, 'Title', 'nada' as new, 'eu'";

		$result = $this->selectData($query);

		$xmlRet = $this->makeNews($result);

		return $xmlRet;
	}

}

$news = new News();

echo $news->selectNews();
?>
